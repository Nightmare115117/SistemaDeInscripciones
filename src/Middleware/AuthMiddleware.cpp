#include "AuthMiddleware.h"
#include "Utils/JWT.h"
#include "Admin/Repo/AdminRepo.h"
#include "DBConfig/DBConfig.h"

using namespace std;
using namespace crow;

void AuthMiddleware::before_handle(
    request& req,
    response& res,
    context& ctx)//
{

    string path = req.url;

    bool publicRoute = path == "/api/test-db" ||
        (path == "/api/login" && req.method == HTTPMethod::POST) ||
        (path == "/api/registro" && req.method == HTTPMethod::POST) ||
        (path == "/api/registro/count" && req.method == HTTPMethod::GET) ||
        (path == "/api/evento" && req.method == HTTPMethod::GET) ||
        (path == "/api/agenda" && req.method == HTTPMethod::GET) ||
        (path == "/api/patrocinadores" && req.method == HTTPMethod::GET) ||
        (path == "/api/correos/estado" && req.method == HTTPMethod::GET) ||
        (path == "/api/universidades" && req.method == HTTPMethod::GET);//

    if (publicRoute) {
        return;
    }

    auto auth = req.get_header_value("Authorization");

    if(auth.empty())
    {
        res.code = 401;
        res.set_header("Content-Type", "application/json");
        res.write(R"({"detail":"Falta Authorization"})");
        res.end();
        return;
    }

    if(auth.find("Bearer ") != 0)
    {
        res.code = 401;
        res.set_header("Content-Type", "application/json");
        res.write(R"({"detail":"Formato incorrecto"})");
        res.end();
        return;
    }

    string token = auth.substr(7);

    JWT::TokenInfo info = JWT::validarToken(token);

    if(!info.valido)
    {
        res.code = 401;
        res.set_header("Content-Type", "application/json");
        res.write(R"({"detail":"Token invalido o expirado"})");
        res.end();
        return;
    }

    DBConfig dbConfig;
    AdminRepo repo = AdminRepo(dbConfig);

    try {
        repo.findById(info.adminId);
    } catch (const exception& e) {
        res.code = 403;
        res.set_header("Content-Type", "application/json");
        res.write(R"({"detail":"Acceso denegado"})");
        res.end();
        return;
    }

    ctx.adminId = info.adminId;
    ctx.username = info.username;
    ctx.role = info.role;
    ctx.authenticated = true;
}

void AuthMiddleware::after_handle(request& reg, response& res, context& ctx) {

}