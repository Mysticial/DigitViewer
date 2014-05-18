bool is_slash(wchar_t ch){
    return ch == L'/' || ch == L'\\';
}

void launch(const char *name){
    ymo_print_astr("\n");
    ymo_print_astr("Auto-Selecting: ");
    ymo_print_astr(name);
    ymo_print_astr("\n\n");

    std::string path("\"");
    path += global_argv[0];

    while (path.size() > 0 && !is_slash(path.back()))
        path.pop_back();

    path += "Binaries/";
    path += name;
    path += ".out\"";

//    path = escape_cmd(path);

    ymo_print_astr(path.c_str());
    ymo_print_astr("\n\n\n");

    ymo_set_color_Y(1);
    ymo_print_astr("Launching Digit Viewer...\n");
    ymo_set_color_G(0);
    ymo_print_astr("================================================================\n\n");
    ymo_set_color_W();

    int ret = system(path.c_str());
    ymo_set_color_default();
    exit(ret);
}
