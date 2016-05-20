/*! @file file_change_notifier.cpp
 * */
#include "file_change_notifier.h"

#include <algorithm>

file_change_notifier::
file_change_notifier(full_path_t file_path,
                     std::function<void(full_path_t, change_event)> callback,
                     change_event events){
    split_path(file_path,this->directory_path_,this->file_name_);

}

void file_change_notifier::stop_watching() noexcept {

}

void file_change_notifier::split_path(full_path_t full_path,
                                      internal_path_t& directory_path,
                                      internal_path_t& file_name){
    directory_path.resize(512, 0);
    TCHAR *file_namepart_ptr;
    DWORD return_value =
            GetFullPathNameW(full_path.c_str(),
                             directory_path.size(),
                             (internal_path_t::value_type*) directory_path.data(),/* Cast won't be needed after C++17 */
                             &file_namepart_ptr);
    if (return_value == 0 || return_value >= directory_path.size()){
        throw std::runtime_error("full_path too long in file_change_notifier");
    }
    file_name = file_namepart_ptr;
    directory_path.resize(return_value - file_name.size());
}
