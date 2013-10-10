/* ymf_APIs_file_Default_internals.h - File IO - Default
 * 
 * Author           : Alexander J. Yee
 * Date Created     : 08/05/2010
 * Last Modified    : 07/26/2011
 * 
 */

#pragma once
#ifndef _ymf_APIs_file_Default_internals_H
#define _ymf_APIs_file_Default_internals_H
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//  Dependencies
#include "ymf_APIs_file_Default_headers.h"
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
void yma_APIs_file(){
    ymo_print_label("File IO:   \t","Default",'R');
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
ym_pL ym_file_BLOCK_SIZE = (ym_pL)1 << 30;
ym_pL ym_file_set_file_length_fails = 0;
void ym_file_print_error(){
    ymo_print_wstr(L"errno: ");
    ymo_print_uL(errno);
    ymo_print_wstr(L"\n");
}
int ym_file_get_error_code(){
    return errno;
}
////////////////////////////////////////////////////////////////////////////////
void ym_file_makedir(const wchar_t *path){
    char *path_a = ymb_str_wa_copy_t(NULL,path);
#ifdef _MSC_VER
    _mkdir(path_a);
#else
    mkdir(path_a,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    ym_ah_free(path_a);
}
void ym_file_rename_pp(const wchar_t *oldname,const wchar_t *newname){
    char *path_o = ymb_str_wa_copy_t(NULL,oldname);
    char *path_n = ymb_str_wa_copy_t(NULL,newname);

//    ymo_print_astr(path_o); cout << endl;
//    ymo_print_astr(path_n); cout << endl;
    while (rename(path_o,path_n)){
        ymo_warning("Unable to rename file.");
        ymo_print_astr("\n\n");
        ymo_print_wstr(newname);
        ymo_print_astr("\n\n");
        ymo_print_astr("Re-attempting...");
        ym_pause();
    }

    ym_ah_free(path_o);
    ym_ah_free(path_n);
}
ym_fL ym_file_getsize(const wchar_t *path){
    ym_file file;
    ym_file_open(NULL,&file,path);
//    FILE *file = ym_fopen_t(path,L"rb");

    char buf;

    ym_fL L;
    ym_fL H = 1;

    //  Find upper bound for file size.
    while (1){
        if (ym_file_setptr_r(&file,H) != 0 || ym_file_rd(&file,&buf,1) == 0){
            break;
        }
        H <<= 1;
    }
    L = H >> 1;

    //  Binary Search the file size
    while (H - L > 1){
        ym_fL m = (L + H) >> 1;
        if (ym_file_setptr_r(&file,m) != 0 || ym_file_rd(&file,&buf,1) == 0){
            H = m;
        }else{
            L = m;
        }
    }

    if (L == 0){
        if (ym_file_setptr_r(&file,0) != 0 || ym_file_rd(&file,&buf,1) == 0){
            return 0;
        }
    }

    return H;
}
int ym_file_exist(const wchar_t *path){
    char *path_a = ymb_str_wa_copy_t(NULL,path);

    FILE *file = fopen(path_a,"rb");
    if (file == NULL){
        ym_ah_free(path_a);
        return 0;
    }
    fclose(file);
    ym_ah_free(path_a);
    return 1;
}
void ym_file_delete_p(const wchar_t *path){
    char *path_a = ymb_str_wa_copy_t(NULL,path);

    remove(path_a);
    //while (remove(path_a)){
    //    ymo_set_color_R(1);
    //    ymo_print_astr("\n");
    //    ymo_print_astr("Unable to delete file.");
    //    ymo_print_astr("\n\n");
    //    ymo_print_astr("\n");
    //    ymo_print_astr("Re-attempting...");
    //    ymo_print_astr("\n");
    //    ymo_set_color_W();
    //    ym_pause();
    //    continue;
    //}

    ym_ah_free(path_a);
}
int ym_file_check_directory(ym_heap *ah,const wchar_t *directory){
    //Returns:
    // - 0  if directory is writable
    // - 1  if directory is not writable

    //  Build the path
    ym_sbw path;
    ym_sbw_init(ah,&path);

    //  Push the base path
    if (directory != NULL && directory[0] != L'\0'){
        ym_sbw_push_wstr(ah,&path,directory);

        //  Ensure the path ends with a slash.
        wchar_t last = path.buffer[path.len - 1];
        if (last != L'/' && last != L'\\')
            ym_sbw_push_char(ah,&path,L'/');
    }

    //  Push the name
    ym_sbw_push_wstr(ah,&path,L"pathcheck.ysf");
    ym_sbw_push_char(ah,&path,L'\0');

//    ymo_print_astr("\n");
//    ymo_print_wstr(path.buffer);

    ym_file file;
    if (ym_file_create_r(ah,&file,0,path.buffer))
        return 1;
//    FILE *file = ym_fopen(path.buffer,L"wb+");
//    if (file == NULL)
//        return 1;

    ym_file_delete(&file);
//    ym_fclose(file);
//    ym_fdelete_t(path.buffer);

    ym_sbw_kill(&path);
    return 0;
}
////////////////////////////////////////////////////////////////////////////////
int ym_file_open_r(ym_heap *ah,ym_file *file,const wchar_t *path){
    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);
    
    char *path_a = ymb_str_wa_copy_t(NULL,path);
    file->filehandle = fopen(path_a,"rb");
    ym_ah_free(path_a);

    if (file->filehandle == NULL){
        ym_ah_free(file->path);
        return 1;
    }

    return 0;
}
//int ym_file_openwr_r(ym_heap *ah,ym_file *file,const wchar_t *path){
//    //  Copy the path
//    file->path = ymb_str_w_copy_t(ah,path);
//    
//    char *path_a = ymb_str_wa_copy_t(NULL,path);
//    file->filehandle = fopen(path_a,"wb+");
//    ym_ah_free(path_a);
//    return file->filehandle == NULL;
//}
void ym_file_open(ym_heap *ah,ym_file *file,const wchar_t *path){
    while (ym_file_open_r(ah,file,path)){
        ymo_print_astr("\n");
        ymo_set_color_R(1);
        ymo_print_astr("Unable to open or create file.");
        ymo_print_astr("\n\n");
        ymo_print_wstr(path);
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }
}
//void ym_file_openwr(ym_heap *ah,ym_file *file,const wchar_t *path){
//    while (ym_file_openwr_r(ah,file,path)){
//        ymo_print_wstr(L"\n");
//        ymo_set_color_R(1);
//        ymo_print_wstr(ymo_str_File_Fail_Open);
//        ymo_print_wstr(L"\n\n");
//        ymo_print_wstr(path);
//        ymo_print_wstr(L"\n");
//        ymo_print_wstr(ymo_str_Reattempting);
//        ymo_print_wstr(L"\n");
//        ymo_set_color_W();
//        ym_pause();
//        continue;
//    }
//}
void ym_file_close(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    while (fclose(file->filehandle)){
        ymo_set_color_R(1);
        ymo_error("Unable to close file.");
//        ymo_print_astr("\n");
//        ymo_print_astr("Re-attempting...");
//        ymo_print_astr("\n");
//        ymo_set_color_W();
//        ym_pause();
//        continue;
    }

    ym_ah_free(file->path);
}
int ym_file_create_r(ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path){
    //  Copy the path
    file->path = ymb_str_w_copy_t(ah,path);
    
    char *path_a = ymb_str_wa_copy_t(NULL,path);
    file->filehandle = fopen(path_a,"wb+");
    ym_ah_free(path_a);

    if (file->filehandle == NULL){
        ym_ah_free(file->path);
        return 1;
    }

    return 0;
}
void ym_file_create(ym_heap *ah,ym_file *file,ym_fL bytes,const wchar_t *path){
    while (ym_file_create_r(ah,file,bytes,path)){
        ymo_print_astr("\n");
        ymo_set_color_R(1);
        ymo_print_astr("Unable to create file.");
        ymo_print_astr("\n\n");
        ymo_print_wstr(path);
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }
}
void ym_file_delete(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    while (fclose(file->filehandle)){
        ymo_set_color_R(1);
        ymo_error("Unable to close file.");
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }

    ym_file_delete_p(file->path);
    ym_ah_free(file->path);
}
void ym_file_force_delete(ym_file *file){
    if (file == NULL || file->path == NULL)
        return;

    fclose(file->filehandle);
        
    char *path_a = ymb_str_wa_copy_t(NULL,file->path);
    remove(path_a);
    ym_ah_free(path_a);

    ym_ah_free(file->path);
}
void ym_file_rename(ym_heap *ah,ym_file *file,const wchar_t *path){
    while (fclose(file->filehandle)){
        ymo_set_color_R(1);
        ymo_error("Unable to close file.");
        ymo_print_astr("\n");
        ymo_print_astr("Re-attempting...");
        ymo_print_astr("\n");
        ymo_set_color_W();
        ym_pause();
        continue;
    }

    ym_file_rename_pp(file->path,path);

    ym_ah_free(file->path);
    ym_file_open(ah,file,path);
}
////////////////////////////////////////////////////////////////////////////////
int ym_file_setptr_r(ym_file *file,ym_fL offset){
#ifdef _MSC_VER
    return _fseeki64(file->filehandle,offset,SEEK_SET);
#else
    return fseeko(file->filehandle,offset,SEEK_SET);
#endif
//    fpos_t pos = (fpos_t)offset;
//    return fsetpos(file->filehandle,&pos);
}
void ym_file_setptr(ym_file *file,ym_fL offset){
    if (ym_file_setptr_r(file,offset)){
        ym_file_print_error();
        ymo_error("Unable to move file pointer.");
    }
}
ym_fL ym_file_getptr(ym_file *file){
#ifdef _MSC_VER
    return _ftelli64(file->filehandle);
#else
    return ftello(file->filehandle);
#endif
    //fpos_t pos;
    //fgetpos(file->filehandle,&pos);
    //return (ym_fL)pos;
}
ym_pL ym_file_wr(ym_file *file,const void *T,ym_pL bytes){
    return fwrite(T,1,(size_t)bytes,file->filehandle);
}
ym_pL ym_file_rd(ym_file *file,void *T,ym_pL bytes){
    return fread(T,1,(size_t)bytes,file->filehandle);
}
void ym_file_flush(ym_file *file){
    fflush(file->filehandle);
}
ym_pL ym_file_wr_u16(ym_heap *ah,ym_file *file,const wchar_t *str){
    //  Writes as a UTF16 string.
    ym_pL length = ymb_str_w_len(str);

    if (sizeof(wchar_t) == sizeof(ym_u16)){
        ym_pL bytes = ym_file_wr(file,str,length * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        ym_u16 *buffer = (ym_u16*)ym_ah_malloc(ah,length * sizeof(ym_u16),0);
        for (ym_pL c = 0; c < length; c++){
            buffer[c] = str[c];
        }
        ym_pL bytes = ym_file_wr(file,buffer,length * sizeof(ym_u16));
        ym_ah_free(buffer);
        return bytes / sizeof(ym_u16);
    }
}
ym_pL ym_file_rd_u16(ym_heap *ah,ym_file *file,wchar_t *str,ym_pL L){
    //  Reads as a UTF16 string.
    memset(str,0,L * sizeof(wchar_t));

    if (sizeof(wchar_t) == sizeof(ym_u16)){
        ym_pL bytes = ym_file_rd(file,str,L * sizeof(wchar_t));
        return bytes / sizeof(wchar_t);
    }else{
        ym_u16 *buffer = (ym_u16*)ym_ah_malloc(ah,L * sizeof(ym_u16),0);
        ym_pL bytes = ym_file_rd(file,buffer,L * sizeof(ym_u16));
        for (ym_pL c = 0; c < L; c++){
            str[c] = buffer[c];
        }
        ym_ah_free(buffer);
        return bytes / sizeof(ym_u16);
    }
}
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#endif
