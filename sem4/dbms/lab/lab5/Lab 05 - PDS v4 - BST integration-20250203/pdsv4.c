// IMT2023111
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdsv4.h"
#include "contact.h"
struct PDS_RepoInfo repo_handle;
void bst_idx_load(struct BST_Node* root, FILE* fp);

// pds_create
// Open the data file and index file in "wb" mode
// Function to use: fopen
// Initialize index file by storing "0" to indicate there are zero entries in index file
// Function to use: fwrite
// close the files
//
// Return values:
//
// PDS_FILE_ERROR - if fopen returns NULL
// PDS_SUCCESS - function completes without any error
//
int pds_create(char* repo_name) {
    FILE* fi;
    FILE* fd;
    char datafile[35], ndxfile[35];
    strcpy(datafile, repo_name);
    strcat(datafile, ".dat");
    strcpy(ndxfile, repo_name);
    strcat(ndxfile, ".ndx");
    // strcpy(repo_handle.pds_name, repo_name);

    fi = fopen(ndxfile, "wb");
    fd = fopen(datafile, "wb");
    int key = 0;
    fwrite(&key, sizeof(int), 1, fi);
    if (fd == NULL || fi == NULL) {
        printf("Error opening file");
        return PDS_FILE_ERROR;
    }
    fclose(fi);
    fclose(fd);

    return PDS_SUCCESS;
};

// pds_load_ndx
// This is Internal function used by pds_open
// Read the number of records form the index file
// [CHANGE] Set up a loop to load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each
// index entry. Unlike array where you could load entire index, for BST, you add index entries
// one by one by reading the index file one by one in a loop
// (see bst_demo.c how to call bst_add_node in a loop)
// Function to use: fread in a loop
// Function to use: bst_add_node in the same loop
//
// Return values:
//
// PDS_FILE_ERROR - if the number of entries is lesser than the number of records
// PDS_SUCCESS - function completes without any error
int pds_load_ndx() {
    if (repo_handle.pds_ndx_fp == NULL) {
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_ndx_fp, 0, SEEK_SET);
    if (fread(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp) < 1) {
        return PDS_FILE_ERROR;
    }
    int i;
    for (i = 0; i < repo_handle.rec_count; i++) {
        struct PDS_NdxInfo ndx;
        if (fread(&ndx, sizeof(struct PDS_NdxInfo), 1, repo_handle.pds_ndx_fp) < 1) {
            return PDS_FILE_ERROR;
        }
        int* offset = (int*)malloc(sizeof(int));
        *offset = ndx.offset;
        bst_add_node(&repo_handle.ndx_root, ndx.key, offset);
    }
    if (i != repo_handle.rec_count)
        return PDS_FILE_ERROR;

    return PDS_SUCCESS;
};

// pds_open
// Open the data file and index file in rb+ mode
// Function to use: fopen
// Update the fields of PDS_RepoInfo appropriately
// Read the number of records form the index file
// Function to use: fread
// Load the index into the array and store in ndx_array by reading index entries from the index file
// // Function to use: fread
// Close only the index file
// Function to use: fclose
//
// Return values:
//
// PDS_FILE_ERROR - if fopen returns NULL
// PDS_REPO_ALREADY_OPEN - if repo_status is PDS_REPO_OPEN
// PDS_SUCCESS - function completes without any error
int pds_open(char* repo_name, int rec_size) {
    FILE* fi;
    FILE* fd;
    char datafile[35], ndxfile[35];
    strcpy(datafile, repo_name);
    strcat(datafile, ".dat");
    strcpy(ndxfile, repo_name);
    strcat(ndxfile, ".ndx");

    if (PDS_REPO_OPEN == repo_handle.repo_status) {
        return PDS_REPO_ALREADY_OPEN;
    }

    fd = fopen(datafile, "rb+");
    fi = fopen(ndxfile, "rb+");

    if (fd == NULL) {
        return PDS_FILE_ERROR;
    }
    if (fi == NULL) {
        fclose(fd);
        return PDS_FILE_ERROR;
    }

    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.pds_data_fp = fd;
    repo_handle.pds_ndx_fp = fi;
    repo_handle.repo_status = PDS_REPO_OPEN;
    repo_handle.rec_size = rec_size;

    // int numrec;
    // fread(&numrec, sizeof(int), 1, fi);
    // repo_handle.rec_count = numrec;
    if (pds_load_ndx() == PDS_FILE_ERROR) {
        return PDS_FILE_ERROR;
    }
    fclose(fi);
    return PDS_SUCCESS;
};

// put_rec_by_key
// Seek to the end of the data file
// Function to use: fseek
// Identify the current location of file pointer
// Function to use: ftell
// Create new index entry using the key and ftell value
// [CHANGE] Add index entry to BST by calling bst_add_node (see bst_demo.c how to call bst_add_node)
// Function to use: bst_add_node
// Increment record count
// Write the key
// Function to use: fwrite
// Write the record
// Function to use: fwrite
//
// Return values:
//
// PDS_REPO_NOT_OPEN - if repo_status is not PDS_REPO_OPEN
// PDS_ADD_FAILED - if fwrite fails OR duplicate record is found
// PDS_SUCCESS - function completes without any error
int put_rec_by_key(int key, void* rec) {
    struct Contact* contact = (struct Contact*)rec;
    if (repo_handle.repo_status != PDS_REPO_OPEN) {
        return PDS_REPO_NOT_OPEN;
    }
    if (repo_handle.pds_data_fp == NULL) {
        return PDS_FILE_ERROR;
    }
    fseek(repo_handle.pds_data_fp, 0, SEEK_END);
    int* loc = (int*)malloc(sizeof(int));
    *loc = ftell(repo_handle.pds_data_fp);
    // struct PDS_NdxInfo ndx = {key, loc};
    if (bst_add_node(&repo_handle.ndx_root, key, loc) != BST_SUCCESS) {
        return PDS_ADD_FAILED;
    }
    repo_handle.rec_count++;
    if (fwrite(&key, sizeof(int), 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;
    if (fwrite(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) < 1)
        return PDS_ADD_FAILED;

    return PDS_SUCCESS;
};

// get_rec_by_key
// [CHANGE] Search for index entry in BST
// Function to use: bst_search
// Seek to the file location based on offset in index entry
// Function to use: fseek
// Read the key at the current location
// Function to use: fread
// Read the record from the current location
// Function to use: fread
//
// Return values:
//
// PDS_REPO_NOT_OPEN - if repo_status is not PDS_REPO_OPEN
// PDS_REC_NOT_FOUND - if the key is not found
// PDS_SUCCESS - function completes without any error
int get_rec_by_key(int key, void* rec) {
    if (repo_handle.repo_status != PDS_REPO_OPEN)
        return PDS_REPO_NOT_OPEN;
    struct BST_Node* result = bst_search(repo_handle.ndx_root, key);

    if (result == NULL) {
        return PDS_REC_NOT_FOUND;
    }
    int offset = *((int*)(result->data));
    fseek(repo_handle.pds_data_fp, offset, SEEK_SET);
    int keyfound;
    fread(&keyfound, sizeof(int), 1, repo_handle.pds_data_fp);
    if (key != keyfound) {
        return PDS_REC_NOT_FOUND;
    }
    fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp);
    return PDS_SUCCESS;
}

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Function to use: fopen
// Store the number of records
// Function to use: fwrite
// [CHANGE] Unload the index into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// See function bst_print in bst.c to see how to do tree traversal
// Function to use: fwrite
// Free the BST and set pointer to NULL
// Function to use: bst_free
// Close the index file and data file
// Function to use: fclose
//
// Return values:
//
// [CHANGE] PDS_REPO_NOT_OPEN - if repo_status is not PDS_REPO_OPEN or if repo_name does not match with name stored in repo_info
// PDS_NDX_SAVE_FAILED - if fopen or fwrite fails
// PDS_SUCCESS - function completes without any error
int pds_close(char* repo_name) {
    if (repo_handle.repo_status != PDS_REPO_OPEN)
        return PDS_REPO_NOT_OPEN;

    if (strcmp(repo_handle.pds_name, repo_name) != 0)
        return PDS_REPO_NOT_OPEN;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    strcat(filename, ".ndx");
    repo_handle.pds_ndx_fp = fopen(filename, "wb");
    if (repo_handle.pds_ndx_fp == NULL)
        return PDS_NDX_SAVE_FAILED;

    fwrite(&repo_handle.rec_count, sizeof(int), 1, repo_handle.pds_ndx_fp);
    bst_idx_load(repo_handle.ndx_root, repo_handle.pds_ndx_fp);
    bst_free(repo_handle.ndx_root);
    repo_handle.ndx_root = NULL;
    fclose(repo_handle.pds_data_fp);
    fclose(repo_handle.pds_ndx_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}

// get_rec_by_field
// Brute-force retrieval using an arbitrary search value
// 	search_count = 0
// 	fread key from data file until EOF
//	search_count++
//  fread the record
//  Invoke the matcher using current record and search value
//	if mathcher returns success, return the current record, else continue the loop
// end loop
//
// Return values:
//
// PDS_REPO_NOT_OPEN - if repo_status is not PDS_REPO_OPEN
// PDS_REC_NOT_FOUND - if the key is not found
// PDS_SUCCESS - function completes without any error
int get_rec_by_field(void* searchvalue, void* rec, int (*matcher)(void* rec, void* searchvalue), int* comp_count) {
    if (repo_handle.repo_status != PDS_REPO_OPEN) {
        return PDS_REPO_NOT_OPEN;
    }

    FILE* fp;
    fp = repo_handle.pds_data_fp;
    fseek(fp, 0, SEEK_SET);
    *comp_count = 0;

    int filekey;
    while (fread(&filekey, sizeof(int), 1, fp) > 0) {
        if (fread(rec, repo_handle.rec_size, 1, fp) != 1)
            break;

        (*comp_count)++;
        if (matcher(rec, searchvalue) == 0) {
            return PDS_SUCCESS;
        }
    }

    return PDS_REC_NOT_FOUND;
}

void bst_idx_load(struct BST_Node* root, FILE* fp) {
    // Pre-order traversal
    if (root == NULL)
        return;
    else {
        int* data = (int*)root->data;
        struct PDS_NdxInfo ndx = { root->key, *data };
        fwrite(&ndx, sizeof(struct PDS_NdxInfo), 1, fp);
        bst_idx_load(root->left_child, fp);
        bst_idx_load(root->right_child, fp);
    }
}