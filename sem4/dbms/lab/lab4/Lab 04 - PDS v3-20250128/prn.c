int get_rec_by_field(void *searchvalue, void *rec, int (*matcher)(void *rec, void *searchvalue), int *comp_count)
{
    if (repo_handle.repo_status != PDS_REPO_OPEN)
    {
        return PDS_REPO_NOT_OPEN;
    }

    fseek(repo_handle.pds_data_fp, 0, SEEK_SET); // Start reading from the beginning of the file
    *comp_count = 0;

    while (1)
    {
        int stored_key;

        // Read the key
        if (fread(&stored_key, sizeof(int), 1, repo_handle.pds_data_fp) != 1)
        {
            break; // End of file reached
        }

        // Read the record
        if (fread(rec, repo_handle.rec_size, 1, repo_handle.pds_data_fp) != 1)
        {
            break; // Failed to read a record
        }

        (*comp_count)++;

        // Use the matcher function to compare the record and the search value
        if (matcher(rec, searchvalue) == 0)
        {
            return PDS_SUCCESS; // Found the record
        }
    }

    return PDS_REC_NOT_FOUND; // Record not found
}