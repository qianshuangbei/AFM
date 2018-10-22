#include "nanomodule.h"

Nanodata::Nanodata(){
}

///////////---date---/////////////////
bool Nanodata::has_date(){
    if(_nano.date != "")
        return true;
    else
        return false;
}
void Nanodata::set_date(const QString &input){
    _nano.date = input;
}
QString Nanodata::date(){
    return _nano.date;
}
///////////---description---/////////////////
bool Nanodata::has_description(){
    if(_nano.description != "")
        return true;
    else
        return false;
}
void Nanodata::set_description(const QString &input){
    _nano.description = input;
}
QString Nanodata::description(){
    return _nano.description;
}
///////////---rate---/////////////////
bool Nanodata::has_rate(){
    if(_nano.rate != "")
        return true;
    else
        return false;
}
void Nanodata::set_rate(const QString &input){
    _nano.rate = input;
}
QString Nanodata::rate(){
    return _nano.rate;
}
///////////---Xsize---/////////////////
bool Nanodata::has_Xsize(){
    if(_nano.Xsize != "")
        return true;
    else
        return false;
}
void Nanodata::set_Xsize(const QString &input){
    _nano.Xsize = input;
}
QString Nanodata::Xsize(){
    return _nano.Xsize;
}
///////////---Ysize---/////////////////
bool Nanodata::has_Ysize(){
    if(_nano.Ysize != "")
        return true;
    else
        return false;
}
void Nanodata::set_Ysize(const QString &input){
    _nano.Ysize = input;
}
QString Nanodata::Ysize(){
    return _nano.Ysize;
}
///////////---linedirection---/////////////////
bool Nanodata::has_linedirection(){
    if(_nano.linedirection != "")
        return true;
    else
        return false;
}
void Nanodata::set_linedirection(const QString &input){
    _nano.linedirection = input;
}
QString Nanodata::linedirection(){
    return _nano.linedirection;
}
///////////---capturedirection---/////////////////
bool Nanodata::has_capturedirection(){
    if(_nano.capturedirection != "")
        return true;
    else
        return false;
}
void Nanodata::set_capturedirection(const QString &input){
    _nano.capturedirection = input;
}
QString Nanodata::capturedirection(){
    return _nano.capturedirection;
}
///////////---driveamp---/////////////////
bool Nanodata::has_driveamp(){
    if(_nano.driveamp != "")
        return true;
    else
        return false;
}
void Nanodata::set_driveamp(const QString &input){
    _nano.driveamp = input;
}
QString Nanodata::driveamp(){
    return _nano.driveamp;
}
///////////---scansize---/////////////////
bool Nanodata::has_scansize(){
    if(_nano.scansize != "")
        return true;
    else
        return false;
}
void Nanodata::set_scansize(const QString &input){
    _nano.scansize = input;
}
QString Nanodata::scansize(){
    return _nano.scansize;
}
///////////---ratio---/////////////////
bool Nanodata::has_ratio(){
    if(_nano.ratio != "")
        return true;
    else
        return false;
}
void Nanodata::set_ratio(const QString &input){
    _nano.ratio = input;
}
QString Nanodata::ratio(){
    return _nano.ratio;
}
///////////---ampsetpoint---/////////////////
bool Nanodata::has_ampsetpoint(){
    if(_nano.ampsetpoint != "")
        return true;
    else
        return false;
}
void Nanodata::set_ampsetpoint(const QString &input){
    _nano.ampsetpoint = input;
}
QString Nanodata::ampsetpoint(){
    return _nano.ampsetpoint;
}

///////////---datalength---/////////////////
uint32_t Nanodata::has_data_length(){
    return _nano.data_length.size();
}
void Nanodata::append_data_length(const int &input){
    _nano.data_length.push_back(input);
}
int Nanodata::data_length(const uint32_t &input){
    return _nano.data_length[input];
}
///////////---dataoffset---/////////////////
uint32_t Nanodata::has_data_offset(){
    return _nano.data_offset.size();
}
void Nanodata::append_data_offset(const int &input){
    _nano.data_offset.push_back(input);
}
int Nanodata::data_offset(const uint32_t &input){
    return _nano.data_offset[input];
}
///////////---datazscale---/////////////////
uint32_t Nanodata::has_data_zscale(){
    return _nano.data_zscale.size();
}
void Nanodata::append_data_zscale(const double &input){
    _nano.data_zscale.push_back(input);
}
double Nanodata::data_zscale(const uint32_t &input){
    return _nano.data_zscale[input];
}
///////////---datasscale---/////////////////
uint32_t Nanodata::has_data_sscale(){
    return _nano.data_sscale.size();
}
void Nanodata::append_data_sscale(const double &input){
    _nano.data_sscale.push_back(input);
}
double Nanodata::data_sscale(const uint32_t &input){
    return _nano.data_sscale[input];
}
///////////---type---/////////////////
bool Nanodata::has_type(){
    if(_nano.type != "")
        return true;
    else
        return false;
}
void Nanodata::set_type(const QString &input){
    _nano.type = input;
}
QString Nanodata::type(){
    return _nano.type;
}
