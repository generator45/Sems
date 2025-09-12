// IMT2023111
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdsv2.h"

struct PDS_RepoInfo repo_handle;

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
int pds_create(char *repo_name)
{
    FILE *fi;
    FILE *fd;
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
    if (fd == NULL || fi == NULL)
    {
        printf("Error opening file");
        return PDS_FILE_ERROR;
    }
    fclose(fi);
    fclose(fd);

    return PDS_SUCCESS;
};

// pds_load_ndx
// Internal function used by pds_open to read index entries into ndx_array
// Function to use: fread
//
// Return values:
//
// PDS_FILE_ERROR - if fread returns less than num_records*8 bytes
// PDS_SUCCESS - function completes without any error
int pds_load_ndx()
{
    FILE *fd;
    fd = repo_handle.pds_ndx_fp;
    size_t nums = fread(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, fd);
    if (nums < repo_handle.rec_count)
    {
        return PDS_FILE_ERROR;
    }
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
int pds_open(char *repo_name, int rec_size)
{
    FILE *fi;
    FILE *fd;
    char datafile[35], ndxfile[35];
    strcpy(datafile, repo_name);
    strcat(datafile, ".dat");
    strcpy(ndxfile, repo_name);
    strcat(ndxfile, ".ndx");

    if (PDS_REPO_OPEN == repo_handle.repo_status)
    {
        return PDS_REPO_ALREADY_OPEN;
    }

    fd = fopen(datafile, "rb+");
    fi = fopen(ndxfile, "rb+");

    if (fd == NULL)
    {
        return PDS_FILE_ERROR;
    }
    if (fi == NULL)
    {
        fclose(fd);
        return PDS_FILE_ERROR;
    }

    strcpy(repo_handle.pds_name, repo_name);
    repo_handle.pds_data_fp = fd;
    repo_handle.pds_ndx_fp = fi;
    repo_handle.repo_status = PDS_REPO_OPEN;
    repo_handle.rec_size = rec_size;

    int numrec;
    fread(&numrec, sizeof(int), 1, fi);
    repo_handle.rec_count = numrec;
    if (pds_load_ndx() == PDS_FILE_ERROR)
    {
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
// Create an index entry with the current data file location using ftell
// Add index entry to ndx_array using offset returned by ftell
// Increment record count
// Write the record at the end of the data file
// Function to use: fwrite
//
// Return values:
//
// PDS_REPO_NOT_OPEN - if repo_status is not PDS_REPO_OPEN
// PDS_ADD_FAILED - if fwrite fails OR duplicate record is found
// PDS_SUCCESS - function completes without any error
int put_rec_by_key(int key, void *rec)
{
    if (repo_handle.repo_status != PDS_REPO_OPEN)
    {
        return PDS_REPO_NOT_OPEN;
    }
    FILE *fp;
    fp = repo_handle.pds_data_fp;
    if (fp == NULL)
    {
        return PDS_FILE_ERROR;
    }
    fseek(fp, 0, SEEK_END);
    int loc = ftell(fp);
    for (int i = 0; i < repo_handle.rec_count; i++)
    {
        if (repo_handle.ndx_array[i].key == key)
        {
            return PDS_ADD_FAILED;
        }
    }
    struct PDS_NdxInfo pds_ndx_info = {key, loc};
    repo_handle.ndx_array[repo_handle.rec_count] = pds_ndx_info;
    repo_handle.rec_count++;
    size_t keyc = fwrite(&key, sizeof(int), 1, fp);
    if (keyc < 1)
    {
        return PDS_ADD_FAILED;
    }
    size_t recc = fwrite(rec, sizeof(rec), 1, fp);
    if (recc < 1)
    {
        return PDS_ADD_FAILED;
    }
    return PDS_SUCCESS;
};

// get_rec_by_key
// Search for index entry in ndx_array
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
int get_rec_by_key(int key, void *rec)
{
    FILE *fp;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    strcat(filename, ".ndx");
    if (repo_handle.repo_status == PDS_REPO_OPEN)
    {
        fp = repo_handle.pds_data_fp;
        if (fp == NULL)
        {
            return PDS_FILE_ERROR;
        }
        int filekey;
        int pos;
        for (int i = 0; i < repo_handle.rec_count; i++)
        {
            if (repo_handle.ndx_array[i].key == key)
            {
                pos = repo_handle.ndx_array[i].offset;
                fseek(fp, pos, SEEK_SET);
                fread(&filekey, sizeof(int), 1, fp);
                if (filekey == key)
                {
                    fread(rec, sizeof(rec), 1, fp);
                    return PDS_SUCCESS;
                }
                else
                {
                    return PDS_REC_NOT_FOUND;
                }
            }
        }
        return PDS_REC_NOT_FOUND;
    }

    return PDS_REPO_NOT_OPEN;
};

// pds_close
// Open the index file in wb mode (write mode, not append mode)
// Function to use: fopen
// Store the number of records
// Function to use: fwrite
// Unload the ndx_array into the index file (overwrite the entire index file)
// Function to use: fwrite
// Close the index file and data file
// Function to use: fclose
//
// Return values:
//
// PDS_NDX_SAVE_FAILED - if fopen or fwrite fails
// PDS_SUCCESS - function completes without any error
int pds_close(char *repo_name)
{
    FILE *fi;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    strcat(filename, ".ndx");
    fi = fopen(filename, "wb");
    if (fi == NULL)
    {
        printf("Error opening file %s\n", filename);
        return PDS_NDX_SAVE_FAILED;
    }
    int numrecs = repo_handle.rec_count;
    fwrite(&numrecs, sizeof(int), 1, fi);
    if (fwrite(repo_handle.ndx_array, sizeof(struct PDS_NdxInfo), repo_handle.rec_count, fi) < repo_handle.rec_count)
    {
        return PDS_NDX_SAVE_FAILED;
    }
    fclose(fi);
    fclose(repo_handle.pds_data_fp);
    repo_handle.repo_status = PDS_REPO_CLOSED;
    return PDS_SUCCESS;
}
