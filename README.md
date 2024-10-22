# bigfile
Make and verify big files
suitable for use in verifying flash drives and sd cards
Usage:

mkbigfile  <base_directory> <file_size> <base_name> <max_files>

Example to  create  10 files of 1 mb size in directory ./test/

mkbigfile ./test/ 1048576 bigfile 10
