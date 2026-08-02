#include "AuthMiddleware.h"
#include "Utils/JWT.h"
#include "Admin/Repo/AdminRepo.h"
#include "DBConfig/DBConfig.h"

using namespace std;
using namespace crow;

void AuthMiddleware::before_handle(
    request& req,
    response& res,
    context& ctx)
{

    string path = req.url;

    if (
        (path == "/api/test-db") || //prueba para la Coneccion a la DB
        (path == "/api/registros" && req.method == HTTPMethod::POST) || //Ruta insersion de Registros
        (path == "/api/equipos" && req.method == HTTPMethod::POST) ||  //Ruta insersion de Equipos
        (path == "/api/registro/count" && req.method == HTTPMethod::GET) || //Ruta para contar los registros
        (path == "/api/alumno" && req.method == HTTPMethod::POST) ||    //Ruta paara insertar alumnos
        (path == "/api/universidades" && req.method == HTTPMethod::GET) //Ruta para listar las universidades
        ) 
    {
        return;
    }

    auto auth = req.get_header_value("Authorization");

    if(auth.empty())
    {
        res.code = 401;
        res.write("Falta Authorization");
        res.end();
        return;
    }

    if(auth.find("Bearer ") != 0)
    {
        res.code = 401;
        res.write("Formato incorrecto");
        res.end();
        return;
    }

    string token = auth.substr(7);

    JWT::TokenInfo info = JWT::validarToken(token);

    if(!info.valido)
    {
        res.code = 401;
        res.write("JWT invalido");
        res.end();
        return;
    }

    DBConfig dbConfig;
    AdminRepo repo = AdminRepo(dbConfig);

    try {
        repo.findById(info.adminId);
    } catch (const exception& e) {
        res.code = 403;
        res.write("Acceso Denegado");
        res.end();
        return;
    }

    ctx.adminId = info.adminId;
    ctx.username = info.username;
    ctx.role = info.role;



}

void AuthMiddleware::after_handle(request& reg, response& res, context& ctx) {

}