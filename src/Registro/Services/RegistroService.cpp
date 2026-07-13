#include "RegistroService.h"

using namespace std;


vector<RegistroModel> RegistroService::findAll() const {
    return repo.findAll();
}