// IMT2023111
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pds.h"
struct PDS_RepoInfo repo_handle;

// Open data files as per the following convention
// If repo_name is "demo", then data file should be "demo.dat"
// Initialize other members of PDS_RepoInfo global variable
// Open the file in "ab+" mode
// Append: open or create a file for update in binary mode;
// File read/write location is placed at the end of the file
//////////////////////////////////////////////////////
// Functions to use:
// fopen in binary append mode
//////////////////////////////////////////////////////
int pds_open(char *repo_name, int rec_size)
{
    FILE *fp;
    char filename[35];
    strcpy(filename, repo_name);
    strcat(filename, ".dat");
    strcpy(repo_handle.pds_name, filename);

    fp = fopen(filename, "ab+");
    if (fp == NULL)
    {
        printf("Error opening file %s\n", filename);
        return PDS_FILE_ERROR;
    }

    repo_handle.pds_data_fp = fp;
    repo_handle.rec_size = rec_size;
    repo_handle.repo_status = PDS_REPO_OPEN;
    return PDS_SUCCESS;
}

//////////////////////////////////////////////////////
// Store record in the data file
// Seek to the end of the data file using fseek with SEEK_END
// Write the <key,record> pair at the current file location
// Access the necessary fwrite parameters from repo_handle
//////////////////////////////////////////////////////
// Functions to use: Check sample code for usage
// fseek to end of file
// fwrite key
// fwrite record
//////////////////////////////////////////////////////
int put_rec_by_key(int key, void *rec)
{
    FILE *fp;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    if (repo_handle.repo_status == PDS_REPO_OPEN)
    {
        // fp = fopen(filename, "ab+");
        fp = repo_handle.pds_data_fp;
        if (fp == NULL)
        {
            printf("Error opening file %s\n", filename);
            return PDS_FILE_ERROR;
        }
        fseek(fp, 0, SEEK_END);
        fwrite(&key, sizeof(int), 1, fp);
        fwrite(rec, sizeof(rec), 1, fp);
        return PDS_SUCCESS;
    }
    return PDS_FILE_ERROR;
}

//////////////////////////////////////////////////////
// Read the record based on the given key
// Seek to the beginning of the file using fseek with SEEK_SET
// Set up a loop to read <key, record> pair till the matching key is found
// Functions to use: Check sample code for usage
// fseek to beginning of file
// fread key
// fread record
//////////////////////////////////////////////////////
int get_rec_by_key(int key, void *rec)
{
    FILE *fp;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    if (repo_handle.repo_status == PDS_REPO_OPEN)
    {
        // fp = fopen(filename, "ab+");
        fp = repo_handle.pds_data_fp;
        if (fp == NULL)
        {
            printf("Error opening file %s\n", filename);
            return PDS_FILE_ERROR;
        }
        fseek(fp, 0, SEEK_SET);
        int filekey;
        while (fread(&filekey, sizeof(int), 1, fp) > 0)
        {
            if (filekey == key)
            {

                fread(rec, sizeof(rec), 1, fp);
                return PDS_SUCCESS;
            }
            fseek(fp, sizeof(rec), SEEK_CUR);
        }
        return PDS_REC_NOT_FOUND;
    }
    return PDS_FILE_ERROR;
}

// Close all files and reset repo_handle values
// // Functions to use: Check sample code for usage
// fclose all file pointers
int pds_close()
{
    FILE *fp;
    char filename[35];
    strcpy(filename, repo_handle.pds_name);
    if (repo_handle.repo_status == PDS_REPO_OPEN)
    {
        fp = repo_handle.pds_data_fp;
        fclose(fp);
        repo_handle.repo_status = PDS_REPO_CLOSED;
        repo_handle.pds_data_fp = NULL;
        return PDS_SUCCESS;
    }
    return PDS_FILE_ERROR;
}
