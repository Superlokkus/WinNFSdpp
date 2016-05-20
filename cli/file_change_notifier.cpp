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
    directory_path.resize(full_path.size()+64, 0);
    TCHAR *file_namepart_ptr;

    auto call_GetFullPathNameW = [&]() {
        return GetFullPathNameW(full_path.c_str(),
                                directory_path.size(),
                                (internal_path_t::value_type*) directory_path.data(),/* Cast won't be needed after C++17 */
                                &file_namepart_ptr);
    };

    DWORD return_value = call_GetFullPathNameW();
    if (return_value >= directory_path.size()){
        directory_path.resize(return_value, 0);
        call_GetFullPathNameW();
    } else if (return_value == 0){
        throw std::runtime_error("Call of GetFullPathNameW failed with " + GetLastError());
    }
    file_name = file_namepart_ptr;
    directory_path.resize(return_value - file_name.size());
}

void file_change_notifier::start_watching(){

}
