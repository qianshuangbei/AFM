#include "nanomodule.h"

Nanodata::Nanodata(){
}

bool Nanodata::has_date(){
    if(_nano.textinfo.date != "")
        return true;
    else
        return false;
}

bool Nanodata::set_date(const QString date){
    _nano.textinfo.date = date;
}

bool Nanodata::date(){
    return _nano.textinfo.date;
}
