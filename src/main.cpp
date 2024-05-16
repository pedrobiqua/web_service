#include <iostream>
#include <string>
#include "crow.h"
#include <curl/curl.h>
// #include "crow_all.h"

void make_req(){
    // Inicializa o libcurl globalmente
    curl_global_init(CURL_GLOBAL_ALL);

    // Inicializa o objeto CURL
    CURL *curl = curl_easy_init();
    if(curl) {
        // Define a URL para a requisição
        curl_easy_setopt(curl, CURLOPT_URL, "https://viacep.com.br/ws/83260000/json/");

        // Realiza a requisição e armazena a resposta em stdout
        curl_easy_perform(curl); 
        // Arrumar isso depois, quero enviar isso pra outro servidor para 
        // poder fazer a requisição e fazer o algoritmo distribuido

        // Libera os recursos do libcurl
        curl_easy_cleanup(curl);
    }

    // Finaliza o libcurl
    curl_global_cleanup();
}


std::string makeHttpRequest(const std::string& url) {
    CURL *curl;
    CURLcode res;
    std::string response;

    curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        // Configura o callback para armazenar a resposta
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void *buffer, size_t size, size_t nmemb, std::string *responseData) -> size_t {
            responseData->append((char*)buffer, size * nmemb);
            return size * nmemb;
        });
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        res = curl_easy_perform(curl);

        // Verifica se houve erros na requisição
        if(res != CURLE_OK) {
            std::cerr << "Failed to perform HTTP request: " << curl_easy_strerror(res) << std::endl;
        }

        // Libera os recursos do curl
        curl_easy_cleanup(curl);
    }

    return response;
}

// Função para lidar com as requisições POST e salvar as mensagens em um arquivo de log
void handle_post_log(crow::request& req, crow::response& res) {
    // Verifica se o método da requisição é POST
    if (req.method == crow::HTTPMethod::POST) {
        // Habilita o CORS permitindo acesso de todas as origens
        res.set_header("Access-Control-Allow-Origin", "*");

        // Obtém a mensagem do corpo da requisição
        std::string message = req.body;

        make_req();
        
        // Abre o arquivo de log em modo de adição (append)
        std::ofstream log_file("log.txt", std::ios::app);
        
        // Verifica se o arquivo foi aberto com sucesso
        if (log_file.is_open()) {
            // Salva a mensagem no arquivo de log
            log_file << message << std::endl;
            log_file.close();
            
            // Define o código de status da resposta como 200 (OK)
            res.code = 200;
            res.write("Mensagem salva no log");
        } else {
            // Se houve um erro ao abrir o arquivo de log, retorna código de status 500 (Internal Server Error)
            res.code = 500;
            res.write("Erro ao abrir o arquivo de log");
        }
    } else {
        // Se a requisição não for POST, retorna código de status 405 (Method Not Allowed)
        res.code = 405;
        res.write("Método não permitido");
    }
    
    // Encerra a resposta
    res.end();
}

int main() {
    crow::SimpleApp app;

    // Define a rota para lidar com as requisições POST no endpoint /log
    CROW_ROUTE(app, "/log").methods("POST"_method)(handle_post_log);

    app.port(8080).multithreaded().run();

    return 0;
}
