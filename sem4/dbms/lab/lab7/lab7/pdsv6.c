// IMT2023111
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include "pdsv6.h"
#include "contact.h"

struct PDS_DBInfo repo_handle;
void bst_idx_load(struct BST_Node* root, FILE* fp);

//////////////////////////////////////////////////////////////////////////////
// helper_get_table_info
//
// Function return the table_info from the array based on name
//
// Return values:
// Pointer to tableinfo - Use & to return the appropriate element from the array
// NULL - if table_name is not found in the tables array of DBInfo
//
struct PDS_TableInfo* helper_get_table_info(char* table_name) {
    for (int i = 0; i < repo_handle.num_tables; i++) {
        if (!strcmp(table_name, repo_handle.tables[i].table_name))
            return &(repo_handle.tables[i]);
    }
    return NULL;
}

// [NEW]
//////////////////////////////////////////////////////////////////////////////
// pds_create_db
//
// Create a new folder named repo_name
// Function to use: mkdir
//
// Create file named repo_name/repo_name.info
//
// Store the name of database into the info file
// Function to use: fwrite
// close the files
// NOTHING need on PDS_DBInfo
//
// Return values:
//
// PDS_FILE_ERROR - if mkdir returns -1
// PDS_SUCCESS - function completes without any error
//
int pds_create_db(char* dbname) {
    int mstatus = mkdir(dbname, 0777);
    if (mstatus == -1)
        return -1;
    char file[30];
    sprintf(file, "%s/%s.info", dbname, dbname);
    FILE* fp = fopen(file, "wb");
    if (fp == NULL)
        return PDS_FILE_ERROR;
    fwrite(dbname, sizeof(char), sizeof(repo_handle.pds_repo_name), fp);
    fclose(fp);
    return PDS_SUCCESS;
}

// [NEW]
//////////////////////////////////////////////////////////////////////////////
// pds_open_db
//
// Change the director to folder named dbname
// Function to use: chdir
//
// Open the file named dbname.info in rb+
// Update the staus of PDS_DBInfo to PDS_REPO_OPEN
//
// Read the name of database from the info file
// Function to use: fread
//
// Compare value read from info file matches the input dbname
//
// Return values:
//
// PDS_DB_ALREADY_OPEN - if open is being attempted on a db that is already open
// PDS_FILE_ERROR - if chdir returns -1
// PDS_FILE_ERROR - if dbname in info does not match with given name
// PDS_SUCCESS - function completes without any error
//
int pds_open_db(char* dbname) {
    if (repo_handle.repo_status == PDS_DB_OPEN)
        return PDS_DB_ALREADY_OPEN;
    int cstatus = chdir(dbname);
    if (cstatus == -1)
        return PDS_FILE_ERROR;
    char file[30];
    sprintf(file, "%s.info", dbname);
    FILE* fp = fopen(file, "rb+");
    if (fp == NULL)
        return PDS_FILE_ERROR;
    repo_handle.repo_status = PDS_DB_OPEN;
    char value[30];
    int fstatus = fread(value, sizeof(char), sizeof(repo_handle.pds_repo_name), fp);
    if (fstatus == -1)
        return PDS_FILE_ERROR;
    if (strcmp(value, dbname) == 0)
        return PDS_SUCCESS;
    return PDS_FILE_ERROR;
}

// pds_create_table
//
// Open the data file and index file in "wb" mode
// Function to use: fopen
//
// Write 0 to index file
// Function to use: fwrite/
//
// No updates needed to table needed to PDS_TableInfo
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_FILE_ERROR - if fopen returns NULL
// PDS_FILE_ERROR - if fwrite fails
//
// PDS_SUCCESS - function completes without any error
//
// [OLD PROTOTYPE] int pds_create(char *repo_name);
int pds_create_table(char* table_name) {
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;
    FILE* fi;
    FILE* fd;
    char datafile[35], ndxfile[35];
    sprintf(datafile, "%s.dat", table_name);
    sprintf(ndxfile, "%s.ndx", table_name);

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
// Set up a loop to load the index entries into the BST ndx_root by calling bst_add_node repeatedly for each
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
//
// [OLD-PROTOTYPE] int pds_load_ndx();
int pds_load_ndx(struct PDS_TableInfo* tablePtr) {
    if (tablePtr->ndx_fp == NULL)
        return PDS_FILE_ERROR;
    fseek(tablePtr->ndx_fp, 0, SEEK_SET);
    if (fread(&(tablePtr->rec_count), sizeof(int), 1, tablePtr->ndx_fp) < 1) {
        return PDS_FILE_ERROR;
    }
    int i;
    for (i = 0; i < tablePtr->rec_count; i++) {
        struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)malloc(sizeof(struct PDS_NdxInfo));
        if (fread(ndx, sizeof(struct PDS_NdxInfo), 1, tablePtr->ndx_fp) < 1) {
            return PDS_FILE_ERROR;
        }

        bst_add_node(&(tablePtr->ndx_root), ndx->key, ndx);
    }
    if (i != tablePtr->rec_count)
        return PDS_FILE_ERROR;

    return PDS_SUCCESS;
};

//////////////////////////////////////////////////////////////////////////////
// pds_open_table
// Open the data file and index file in rb+ mode
// Function to use: fopen
// Update the fields of PDS_TableInfo appropriately
// Call pds_load_ndx to load the index
// Close only the index file
// Function to use: fclose
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_FILE_ERROR - if fopen returns NULL
// PDS_TABLE_ALREADY_OPEN - if table_status is PDS_TABLE_OPEN
// PDS_SUCCESS - function completes without any error
// int pds_open( char *repo_name, int rec_size );
int pds_open_table(char* table_name, int rec_size) {
    if (repo_handle.repo_status != PDS_DB_OPEN) {
        return PDS_DB_NOT_OPEN;
    }
    FILE* fi;
    FILE* fd;
    char datafile[35], ndxfile[35];
    sprintf(datafile, "%s.dat", table_name);
    sprintf(ndxfile, "%s.ndx", table_name);

    fd = fopen(datafile, "rb+");
    fi = fopen(ndxfile, "rb+");

    if (fd == NULL) {
        return PDS_FILE_ERROR;
    }
    if (fi == NULL) {
        fclose(fd);
        return PDS_FILE_ERROR;
    }
    struct PDS_TableInfo* table_info = helper_get_table_info(table_name);
    if (table_info == NULL) {
        struct PDS_TableInfo* new_table_info = (struct PDS_TableInfo*)malloc(sizeof(struct PDS_TableInfo));
        strcpy(new_table_info->table_name, table_name);
        new_table_info->table_status = PDS_TABLE_OPEN;
        new_table_info->data_fp = fd;
        new_table_info->ndx_fp = fi;
        new_table_info->rec_size = rec_size;
        new_table_info->ndx_root = NULL;
        repo_handle.tables[repo_handle.num_tables] = *new_table_info;
        repo_handle.num_tables++;
        if (pds_load_ndx(new_table_info) == PDS_FILE_ERROR) {
            return PDS_FILE_ERROR;
        }
    }
    else {
        if (table_info->table_status == PDS_TABLE_OPEN)
            return PDS_TABLE_ALREADY_OPEN;
        strcpy(table_info->table_name, table_name);
        table_info->table_status = PDS_TABLE_OPEN;
        table_info->data_fp = fd;
        table_info->ndx_fp = fi;
        table_info->rec_size = rec_size;
        table_info->ndx_root = NULL;
        if (pds_load_ndx(table_info) == PDS_FILE_ERROR) {
            return PDS_FILE_ERROR;
        }
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
// [CHANGE] set the is_deleted flag to REC_NOT_DELETED
// Add index entry to BST by calling bst_add_node (see bst_demo.c how to call bst_add_node)
// Function to use: bst_add_node
// Increment record count
// Write the key
// Function to use: fwrite
// Write the record
// Function to use: fwrite
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_TABLE_NOT_OPEN - if table status is not PDS_TABLE_OPEN
// PDS_ADD_FAILED - if fwrite fails OR duplicate record is found
// PDS_SUCCESS - function completes without any error
// [OLD PROTOTYPE] int put_rec_by_key( int key, void *rec );
int put_rec_by_key(char* table_name, int key, void* rec) {
    struct Contact* contact = (struct Contact*)rec;
    if (repo_handle.repo_status != PDS_DB_OPEN) {
        return PDS_DB_NOT_OPEN;
    }

    struct PDS_TableInfo* table = helper_get_table_info(table_name);

    if (table == NULL)
        return PDS_FILE_ERROR;
    if (table->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    fseek(table->data_fp, 0, SEEK_END);
    int loc = ftell(table->data_fp);
    struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)malloc(sizeof(struct PDS_NdxInfo));
    ndx->key = key;
    ndx->offset = loc;
    ndx->is_deleted = REC_NOT_DELETED;

    if (bst_add_node(&table->ndx_root, key, ndx) != BST_SUCCESS) {
        return PDS_ADD_FAILED;
    }

    // now write to the ndx file here strcuts here
    fseek(table->ndx_fp, 0, SEEK_END);
    fwrite(ndx, sizeof(struct PDS_NdxInfo), 1, table->ndx_fp);

    (table->rec_count)++;
    if (fwrite(&key, sizeof(int), 1, table->data_fp) < 1)
        return PDS_ADD_FAILED;
    if (fwrite(rec, table->rec_size, 1, table->data_fp) < 1)
        return PDS_ADD_FAILED;

    return PDS_SUCCESS;
};

// get_rec_by_key
// Search for index entry in BST
// Function to use: bst_search
//
// Type cast the void *data in the node to struct PDS_NdxInfo *
// [CHANGE] Return PDS_REC_NOT_FOUND if is_deleted_flag is REC_DELETED
// Seek to the file location based on offset in index entry
// Function to use: fseek
// Read the key at the current location
// Function to use: fread
// Read the record from the current location
// Function to use: fread
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_TABLE_NOT_OPEN - if table status is not PDS_TABLE_OPEN
// PDS_REC_NOT_FOUND - if the key is not found or is_deleted_flag is REC_DELETED
// PDS_SUCCESS - function completes without any error
// [OLD PROTOTYPE] int get_rec_by_key( int key, void *rec );
int get_rec_by_key(char* table_name, int key, void* rec) {
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_NOT_OPEN;
    struct PDS_TableInfo* table = helper_get_table_info(table_name);
    struct BST_Node* result = bst_search(table->ndx_root, key);

    if (result == NULL) {
        return PDS_REC_NOT_FOUND;
    }
    struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)(result->data);
    int offset = ndx->offset;
    if (ndx->is_deleted == REC_DELETED) {
        return PDS_REC_NOT_FOUND;
    }
    fseek(table->data_fp, offset, SEEK_SET);
    int keyfound;
    fread(&keyfound, sizeof(int), 1, table->data_fp);
    if (key != keyfound) {
        return PDS_REC_NOT_FOUND;
    }
    fread(rec, table->rec_size, 1, table->data_fp);
    return PDS_SUCCESS;
}

// pds_close
//
// Open the index file in wb mode (write mode, not append mode)
// Function to use: fopen
// Store the number of records
// Function to use: fwrite
// Unload the index into the index file by traversing the BST in pre-order mode (overwrite the entire index file)
// See function bst_print in bst.c to see how to do tree traversal
// Function to use: fwrite
// Free the BST and set pointer to NULL
// Function to use: bst_free
// Close the index file and data file
// Function to use: fclose
//
// Return values:
//
// PDS_DB_NOT_OPEN - If database is not open
// PDS_TABLE_NOT_OPEN - if table status is not PDS_TABLE_OPEN
// PDS_NDX_SAVE_FAILED - if fopen or fwrite fails
// PDS_SUCCESS - function completes without any error
// [OLD PROTOTYPE] int pds_close( char * repo_name );
int pds_close_table(char* table_name) {
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_ALREADY_OPEN;

    struct PDS_TableInfo* table_info = helper_get_table_info(table_name);
    if (table_info->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;
    char filename[35];
    sprintf(filename, "%s.ndx", table_info->table_name);
    table_info->ndx_fp = fopen(filename, "wb");
    if (table_info->ndx_fp == NULL)
        return PDS_NDX_SAVE_FAILED;

    fwrite(&(table_info->rec_count), sizeof(int), 1, table_info->ndx_fp);
    bst_idx_load(table_info->ndx_root, table_info->ndx_fp);
    bst_destroy(table_info->ndx_root);

    table_info->ndx_root = NULL;
    fclose(table_info->data_fp);
    fclose(table_info->ndx_fp);
    table_info->table_status = PDS_TABLE_CLOSED;
    return PDS_SUCCESS;
}

// [NEW]
//////////////////////////////////////////////////////////////////////////////
// pds_close_db
//
// Loop through PDS_TableInfo array
// If the table status is OPEN, then call pds_close_table
// Change the folder to parent folder by calling chdir("..")
int pds_close_db(char* dbname) {
    for (int i = 0; i < repo_handle.num_tables; i++) {
        if (repo_handle.tables[i].table_status == PDS_TABLE_OPEN)
            pds_close_table(repo_handle.tables[i].table_name);
    }
    int cstatus = chdir("..");
    repo_handle.repo_status = PDS_DB_CLOSED;
    return PDS_SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////
// get_rec_by_field
// Brute-force retrieval using an arbitrary search value
// 	search_count = 0
// 	fread key from data file until EOF
//	search_count++
//  fread the record
//  Invoke the matcher using current record and search value
//	if mathcher returns success, return the current record only if not deleted, else continue the loop
// end loop
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_TABLE_NOT_OPEN - if table status is not PDS_TABLE_OPEN
// PDS_REC_NOT_FOUND - if the record is not found or is_deleted_flag is REC_DELETED
// PDS_SUCCESS - function completes without any error
// comp_count should be the number of comparisons being done in the linear search
//
// [OLD PROTOTYPE] int get_rec_by_field(void *searchvalue, void *rec, int (*matcher)(void *rec, void *searchvalue), int *comp_count);
int get_rec_by_field(char* table_name, void* searchvalue, void* rec, int (*matcher)(void* rec, void* searchvalue), int* comp_count) {
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_ALREADY_OPEN;

    struct PDS_TableInfo* table = helper_get_table_info(table_name);
    if (table->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    FILE* fp;
    fp = table->data_fp;
    fseek(fp, 0, SEEK_SET);
    *comp_count = 0;

    int filekey;
    while (fread(&filekey, sizeof(int), 1, fp) > 0) {
        if (fread(rec, table->rec_size, 1, fp) != 1)
            break;

        (*comp_count)++;
        if (matcher(rec, searchvalue) == 0) {
            // not deleted mark
            struct BST_Node* result = bst_search(table->ndx_root, filekey);
            if (result == NULL) {
                return PDS_REC_NOT_FOUND;
            }
            struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)(result->data);
            if (ndx->is_deleted == REC_NOT_DELETED) {
                return PDS_SUCCESS;
            }
        }
    }
    return PDS_REC_NOT_FOUND;
}

void bst_idx_load(struct BST_Node* root, FILE* fp) {
    // Pre-order traversal
    if (root == NULL)
        return;
    else {
        struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)malloc(sizeof(struct PDS_NdxInfo));
        ndx = ((struct PDS_NdxInfo*)root->data);

        fwrite(ndx, sizeof(struct PDS_NdxInfo), 1, fp);
        bst_idx_load(root->left_child, fp);
        bst_idx_load(root->right_child, fp);
    }
}

//////////////////////////////////////////////////////////////////////////////
// delete_rec_by_key
//
// Search for index entry in BST
// Function to use: bst_search
// Type cast the void *data in the node to struct PDS_NdxInfo *
// if is_deleted_flag is REC_NOT_DELETED
// 		Set the is_deleted flag to REC_DELETED
// else
// 		return PDS_REC_NOT_FOUND
//
// Return values:
//
// PDS_DB_NOT_OPEN - if database is not open
// PDS_TABLE_NOT_OPEN - if table status is not PDS_TABLE_OPEN
// PDS_REC_NOT_FOUND - if the key is not found or is already marked as REC_DELETED
// PDS_SUCCESS - function completes without any error
//
// [OLD PROTOTYPE] int delete_rec_by_key( int key );
int delete_rec_by_key(char* table_name, int key) {
    if (repo_handle.repo_status != PDS_DB_OPEN)
        return PDS_DB_ALREADY_OPEN;

    struct PDS_TableInfo* table = helper_get_table_info(table_name);
    if (table->table_status != PDS_TABLE_OPEN)
        return PDS_TABLE_NOT_OPEN;

    struct BST_Node* result = bst_search(table->ndx_root, key);
    if (result == NULL) {
        return PDS_REC_NOT_FOUND;
    }
    struct PDS_NdxInfo* ndx = (struct PDS_NdxInfo*)(result->data);
    if (ndx->is_deleted == REC_DELETED)
        return PDS_REC_NOT_FOUND;
    ndx->is_deleted = REC_DELETED;
    return PDS_SUCCESS;
}

// pds_make_index
//     given data file,
//     create an index file int make_index(FILE *fp)
// {
//     FILE *fi = fopen("index.ndx", "wb");
//     if (fi == NULL)
//     {
//         return PDS_FILE_ERROR;
//     }
//     fseek(fi, 0, SEEK_SET);
//     fseek(fp, 0, SEEK_SET);
//     int num_recs = repo_handle.rec_count;
//     fwrite(&num_recs, sizeof(int), 1, fi);
//     for (int i = 0; i < num_recs; i++)
//     {
//         int key;
//         struct Contact contact;
//         fread(&key, sizeof(int), 1, fp);
//         int offset = ftell(fp);
//         struct PDS_NdxInfo ndx = {key, offset, REC_NOT_DELETED};
//         fwrite(&ndx, sizeof(struct PDS_NdxInfo), 1, fi);
//     }
//     fclose(fi);
//     return PDS_SUCCESS;
// }