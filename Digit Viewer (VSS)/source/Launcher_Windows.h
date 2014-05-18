bool is_slash(wchar_t ch){
    return ch == L'/' || ch == L'\\';
}

void launch(const char *name){
    ymo_print_astr("\n");
    ymo_print_astr("Auto-Selecting: ");
    ymo_print_astr(name);
    ymo_print_astr("\n\n");

    DWORD size;
    wchar_t path_a[4200];
    size = GetModuleFileNameW(NULL,path_a,4096);
    if (size == 0){
        ymo_error("Unable to read file path.");
    }

    std::wstring path(L"\"");

    path += path_a;
    while (path.size() > 0 && !is_slash(path.back()))
        path.pop_back();

    path += L"Binaries\\";

    while (name[0] != '\0'){
        path += (wchar_t)*name++;
    }

    path += L"\"";

    ymo_print_wstr(path.c_str());
    ymo_print_astr("\n\n\n");

    ymo_set_color_Y(1);
    ymo_print_astr("Launching Digit Viewer...\n");
    ymo_set_color_G(0);
    ymo_print_astr("================================================================\n\n");
    ymo_set_color_W();

    int ret = _wsystem(path.c_str());
    if (ret != 0){
        ymo_print_astr("\n\n");
        ym_pause();
    }
    exit(ret);
}
