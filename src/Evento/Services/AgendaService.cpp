#include "AgendaService.h"
#include <stdexcept>

AgendaService::AgendaService(AgendaRepository& repository) : repo(repository) {}
std::vector<AgendaModel> AgendaService::findAll() const { return repo.findAll(); }
AgendaModel AgendaService::insert(const AgendaModel& item) const { validate(item); return repo.insert(item); }
AgendaModel AgendaService::update(const std::string& id, const AgendaModel& item) const { validateId(id); validate(item); return repo.update(id, item); }
bool AgendaService::remove(const std::string& id) const { validateId(id); return repo.remove(id); }
void AgendaService::validate(const AgendaModel& item) {
    if (item.getDia().empty() || item.getInicio().empty() || item.getFin().empty() || item.getTitulo().empty())
        throw std::invalid_argument("La agenda requiere dia, horario y titulo");
}
void AgendaService::validateId(const std::string& id) {
    if (id.empty()) throw std::invalid_argument("El id de agenda no puede estar vacio");
}
