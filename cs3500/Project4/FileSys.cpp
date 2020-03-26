// CPSC 3500: File System
// Implements the file system commands that are available to the shell.

#include <cstring>
#include <string.h>
#include <iostream>
#include <unistd.h>
using namespace std;

#include "FileSys.h"
#include "BasicFileSys.h"
#include "Blocks.h"

// mounts the file system
void FileSys::mount(int sock) {
	bfs.mount();
	curr_dir = 1; //by default current directory is home directory, in disk block #1
	fs_sock = sock; //use this socket to receive file system operations from the client and send back response messages

}

// unmounts the file system
void FileSys::unmount() {
	bfs.unmount();
	close(fs_sock);
}

// make a directory
void FileSys::mkdir(const char *name)
{

	// cast to dirblock_t*
	dirblock_t current; 
	
	// read current directory block
	bfs.read_block(curr_dir, (void *) &current);
	
	// TODO: function checking naming converntions
	for(int i = 0; i < current.num_entries; i++) {
		if (strcmp(name, current.dir_entries[i].name) == 0) {
			cout << "Directory already exists\n";
			return;
		}
	}
	
	// get a free block
	short new_block = bfs.get_free_block();
	// get next directory slot
	short new_entry = get_dir_entry(&current);
	
	// check to make sure dir slot is valid
	// if so, update info of curr_dir
	if (new_entry >= 0) {
		current.dir_entries[new_entry].block_num = new_block;
		current.num_entries++;
		strcpy(current.dir_entries[new_entry].name, name);
	}
	else {
		cout << "NO MORE ROOM IN DIRECTORY: " << curr_dir << endl;
	}
	
	// initialize the new directory: maybe make own function
	struct dirblock_t new_dir_block;
	new_dir_block.magic = DIR_MAGIC_NUM;
	new_dir_block.num_entries = 0;
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		new_dir_block.dir_entries[i].block_num = 0;
	}
	
	// write updated blocks to disk
	bfs.write_block(curr_dir, (void *) &current);
	bfs.write_block(new_block, (void *) &new_dir_block);
}

// switch to a directory
void FileSys::cd(const char *name)
{

	// struct to read current directory properities into
	dirblock_t current, inner;
	bfs.read_block(curr_dir, (void *) &current);
	
	// find directory block
	int dir = find_directory(&current, &inner, const_cast<char*> (name));
	
	if(dir == -1){
		// is a file; might delete this
		return;
	}
	else if(dir == -2){
		cout << "No file or directory named " << name << " found." << endl;
		return;
	}
	
	// all tests passed; moving into direcotry
	cout << "Moving into directoy: " << name << endl;
	curr_dir = current.dir_entries[dir].block_num;
	
	return;
}

// switch to home directory
void FileSys::home() 
{
	cout << "Going home....\n";
	curr_dir = 1;
}

// remove a directory
void FileSys::rmdir(const char *name)
{
	//need to: 
	// check for name; check that its a directory
	// check if sub directory empty, then reclaim block from super block, 
	// delete reference in current directory.
	
	// read block into this buffer
	char *block = new char[BLOCK_SIZE];
	// read current directory block
	bfs.read_block(curr_dir, block);
	
	// struct to read current directory properities into
	dirblock_t current = *(static_cast<dirblock_t *>(static_cast<void *>(block)));
	dirblock_t inner;
	
	int dir = find_directory(&current, &inner, const_cast<char*> (name));
	
	if(dir == -1){
		//cout << "name '" << current->dir_entries[i].name << "' belongs to a file" << endl;
		return;
	}
	else if(dir == -2){
		cout << "No file or directory named " << name << " found." << endl;
		return;
	}
	
	// check if sub directory is empty
	if (inner.num_entries == 0) {
		// reclaim block
		bfs.reclaim_block(current.dir_entries[dir].block_num);

		// delete reference in current directory 
		current.num_entries--;
		
		// write current directory back to disk
		bfs.write_block(curr_dir, (void *) &current);
		return;
	}
	
	cout << "directory not empty, cannot remove\n";

	return;
	
}

// list the contents of current directory
void FileSys::ls()
{
	// struct to read current directory properities into
	dirblock_t current, inner;
	
	// read current directory block
	bfs.read_block(curr_dir, (void *) &current);
	
	// read names of files in directory
	for (int i = 0; i < current.num_entries; i++) {	
						
		// if not empty; determine if a directory or inode by reading block 
		// and reading magic number of block; can use same buffer as above.
		bfs.read_block(current.dir_entries[i].block_num, (void *) &inner);
		
		if (inner.magic == DIR_MAGIC_NUM) { // is directory
			cout << strcat(current.dir_entries[i].name, "/") << endl;
		} 
		else { //is inode
			cout << current.dir_entries[i].name << endl;
		}			
	}
}

// create an empty data file
void FileSys::create(const char *name)
{
	// Need to: check name validity
	//			check for space in system
	// 			check for space in directory
	// 			create inode
	// 			link to current directory
	
	// TODO: function checking naming converntions
	
	// cast to dirblock_t*
	dirblock_t current;
	// read current directory block
	bfs.read_block(curr_dir, (void *) &current);
	
	// get a free block
	short new_block = bfs.get_free_block();
	short new_entry = get_dir_entry(&current);
	
	// get next available directory entry and initalize if true
	if (new_entry >= 0) {
		current.dir_entries[new_entry].block_num = new_block;
		current.num_entries++;
		strcpy(current.dir_entries[new_entry].name, name);
	}
	else {
		cout << "NO MORE ROOM IN DIRECTORY: " << curr_dir << endl;
	}
	
	// initialize the new inode: maybe make own function
	struct inode_t new_inode_block;
	new_inode_block.magic = INODE_MAGIC_NUM;
	new_inode_block.size = 0;
	for (int i = 0; i < MAX_DATA_BLOCKS; i++) {
		new_inode_block.blocks[i] = 0;
	}
	
	// write updated blocks to disk
	bfs.write_block(curr_dir, (void *) &current);
	bfs.write_block(new_block, (void *) &new_inode_block);
}

// append data to a data file
void FileSys::append(const char *name, const char *data)
{
	// need to: make sure file exists and is file
	//			get str length
	//			get data block from inode; check how much room is left
	// 			append
	
	// init
	int str_length, inode_index;
	
	// set up
	str_length = strlen(data);
	
	// first check if data to write is too long
	if (str_length > MAX_FILE_SIZE) {
		cout << "New data is too long to store in this system\n";
	}	
	
	// init structs
	dirblock_t current;
	inode_t inner;
	
	// read current directory block
	bfs.read_block(curr_dir, (void *) &current);
		
	// determine if name belongs to a created file
	inode_index = find_file(&current, &inner, const_cast<char*> (name));
	
	if(inode_index == -1){
		//cout << "name '" << current->dir_entries[i].name << "' belongs to a directory" << endl;
		return;
	}
	else if(inode_index == -2){
		cout << "No file or file named " << name << " found." << endl;
		return;
	}
	
	// if here we know we need to read inode into memccpy
	bfs.read_block(inode_index, (void *) &inner);
	
	// now check current size
	int block_index, new_block_num, blocks_needed, count, data_index;
	datablock_t new_block;
	
	// cases: size is 0; size is full; size is inbetween
	if (inner.size == 0) {
		
		// determine number of blocks needed
		blocks_needed = 1 + (str_length / BLOCK_SIZE);
		cout << "STR_LEN: " << str_length << " BLOCKS NEEDED: " << blocks_needed << endl;
		
		// initialize first block needed
		//new_block_num = bfs.get_free_block();
		//inner.blocks[get_first_entry(&inner)] = new_block_num;
		
		// read inot memory
		//bfs.read_block(new_block_num, (void *) &new_block);
		count = 0;
		//print string
		for (int character = 0; character < str_length; character++) {
			
			// if its time for a new block
			if (character % BLOCK_SIZE == 0) {
				
				cout << inner.size << endl;
				// get next free block and entry
				new_block_num = bfs.get_free_block();
				data_index = get_first_entry(&inner);
				inner.blocks[data_index] = new_block_num;
				//cout << "DISK BLOCK NUM IS: " << new_block_num << " AND INDOE ENETRY IS: " << data_index << endl;
				

				// read this last block back to disk
				bfs.write_block(inner.blocks[data_index], (void *) &new_block);
				
				// read new block into memory
				bfs.read_block(new_block_num, (void *) &new_block);
				
				// reset count for new block
				count = 0;
			}
			
			new_block.data[count] = data[character];
			// cout << new_block.data[count] << endl;
			count++;
		}
		
		bfs.write_block(curr_dir, (void *) &current);
		bfs.write_block(current.dir_entries[inode_index].block_num, (void *) &inner);
		// write to correct block in order
		// for (int block = 0; block < blocks_needed; block++) {
			
			// // read data_block into memory
			// bfs.read_block(inner.blocks[block], (void *) &new_block);
			
			// // write the max number of characters then switch to new block
			// for (int characters = 0; characters < BLOCK_SIZE; characters++) {
				// new_block.data[characters] = data[characters];
				// cout << new_block.data[characters] << endl;
			// }
			// cout << "LINE\n";
			// // write full block back to disk
			// bfs.write_block(inner.blocks[block], (void *) &new_block);
		// }
		
		//block_index = 0;					
		
		//inner.size += str_length;
		//block_index = inner.size;
		//for(int j = 0; j < str_length; j++) {
			//new_block.data[j] = data[j];
		//}
	}
	//else (inner.size + str_length > MAX_FILE_SIZE) {
	//}
	/*
	// read names of files in directory
	for (int i = 0; i < current.num_entries; i++) {	
		int num = inner.size%128;
		
		if (strcmp(current.dir_entries[i].name, name) == 0) { // name matches; check if inode

			// if name matches; determine if a directory or inode by reading block 
			// and reading magic number of block; can use same buffer as above.
			bfs.read_block(current.dir_entries[i].block_num, block);
			inner = *(static_cast<inode_t *>(static_cast<void *>(block)));
			block_index = inner.size;
			if (inner.magic == INODE_MAGIC_NUM) { // is inode
				inode_index = i;
				// if size 0; get block
				if(inner.size % 128 == 0){
					block_index = 0;					
					short n = bfs.get_free_block();
					inner.blocks[0] = n;
					inner.size += str_length;
					block_index = inner.size;
					for(int j = 0; j < str_length; j++) {
						new_block.data[j] = data[j];
					}
				}
				else{
					bfs.read_block(inner.blocks[num], block);
					new_block  = *(static_cast<datablock_t *>(static_cast<void *>(block)));
					
					for(int j = block_index; j < block_index + str_length; j++) {
						new_block.data[j] = data[j - inner.size];
					}
					cout << "inner size: " << inner.size << endl;
					
					bfs.write_block(inner.blocks[num], (void *) &new_block);
					inner.size += str_length;
					//while(inner.size < BLOCK_SIZE){
						//block_index++;
						if(inner.size > BLOCK_SIZE){
							block_index = 0;
							num++;
							char *block_two = new char[BLOCK_SIZE];
							bfs.read_block(inner.blocks[num], block_two);
							new_block  = *(static_cast<datablock_t *>(static_cast<void *>(block_two)));
							short n = bfs.get_free_block();
							inner.blocks[num] = n;
							for(int j = 0; j < str_length; j++) {
								new_block.data[j] = data[j];
							}
							bfs.write_block(inner.blocks[num], (void *) &new_block);
						}
						/*else if(inner.size + str_length > BLOCK_SIZE){
							int x, index;
							x = 0;
							for(int j = inner.size; j < BLOCK_SIZE; j++){
								new_block.data[j] = data[x];
								x++;
								index = x;
							}
							bfs.write_block(inner.blocks[num], (void *) &new_block);
							
							block_index = 0;
							num++;
							char *block_two = new char[BLOCK_SIZE];
							bfs.read_block(inner.blocks[num], block_two);
							new_block  = *(static_cast<datablock_t *>(static_cast<void *>(block_two)));
							short n = bfs.get_free_block();

							inner.blocks[num] = n;
							for(int j = 0; j < (str_length-index); j++){
								new_block.data[j] = data[index];
								index++;
							}
						}
					//}
					

				}	
			} 
			else { //is inode
				cout << "name '" << current.dir_entries[i].name << "' belongs to a file" << endl;
				return;						
			}						
		}
	}
	*/
	
	// write updated blocks to disk
	//bfs.write_block(inner.blocks[0], (void *) &new_block);
	//bfs.write_block(curr_dir, (void *) &current);
	//bfs.write_block(current.dir_entries[inode_index].block_num, (void *) &inner);
	
	return;	
}

// display the contents of a data file
void FileSys::cat(const char *name)
{

	// init
	int file_index;
	
	// struct to read current directory properities into
	dirblock_t current;
	inode_t inner;
	datablock_t new_block;
	
	// read current directory block
	bfs.read_block(curr_dir, (void *) &current);

	// find file index
	file_index = find_file(&current, &inner, const_cast<char*> (name));
	
	// if size 0; get block
	bfs.read_block(inner.blocks[0], (void *) &new_block);
	
	//cout << inner.size << endl;
	for(int j = 0; j < inner.size; j++){
		cout << new_block.data[j];
	}
	
}

// display the first N bytes of the file
void FileSys::head(const char *name, unsigned int n)
{
}

// delete a data file
void FileSys::rm(const char *name)
{
}

// display stats about file or directory
void FileSys::stat(const char *name)
{
}

// HELPER FUNCTIONS (optional)

// print contents of directory; used for testing
void FileSys::print_dir(dirblock_t *directory) 
{
	// test outputs
	cout << "\nMagic: " << directory->magic << endl;
	cout << "num_entries: " << directory->num_entries << endl;	
	
	// read names of files in dirblock_t: make own function
	for (int i = 0; i < MAX_DIR_ENTRIES; i++) {
		cout << "dir_enrty[" << i <<"] is: " << directory->dir_entries[i].name << " and it goes to block: " << directory->dir_entries[i].block_num << endl;
	}
}

// used for finding the next available dir_entry in current 
// directory
short FileSys::get_dir_entry(dirblock_t *directory) 
{
	// get a pointer in current directory to new directory
	for (short i = 0; i < directory->num_entries; i++) {
		if (directory->dir_entries[i].block_num == 0) {
			return i;
		}
	}
}

int FileSys::find_directory(dirblock_t *current, dirblock_t *inner, char* name) {
	// read names of files in directory
	for (int i = 0; i < current->num_entries; i++) {				
		if (strcmp(current->dir_entries[i].name, name) == 0) { // name matches; check if directory
			
			// if name matches; determine if a directory or inode by reading block 
			// and reading magic number of block; can use same buffer as above.
			bfs.read_block(current->dir_entries[i].block_num, (void *) inner);
			
			//inner = *(static_cast<dirblock_t *>(static_cast<void *>(block)));
			if (inner->magic == DIR_MAGIC_NUM) { // is directory
				return i;
			} 
			else { //is inode
				cout << "name '" << current->dir_entries[i].name << "' belongs to a file" << endl;
				return -1;
			}		
		} 		
	}
	
	return -2;
}

int FileSys::find_file(dirblock_t *current, inode_t *inner, char* name) {
	// read names of files in directory
	for (int i = 0; i < current->num_entries; i++) {				
		if (strcmp(current->dir_entries[i].name, name) == 0) { // name matches; check if file
			
			// if name matches; determine if a directory or inode by reading block 
			// and reading magic number of block; can use same buffer as above.
			bfs.read_block(current->dir_entries[i].block_num, (void *) inner);

			if (inner->magic == INODE_MAGIC_NUM) { // is directory
				return i;
			} 
			else { //is directory
				cout << "name '" << current->dir_entries[i].name << "' belongs to a directory" << endl;
				return -1;
			}		
		} 		
	}
	
	return -2;
}

int FileSys::get_first_entry(inode_t *inode) 
{
	int index = inode->size;
	inode->size++;
	return index;
}
