#include <iostream>
#include <string>
#include "sqlite3.h"


static int callback(void* data, int argc, char** argv, char** azColName) {
    for (int i = 0; i < argc; i++) {
        std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << "\n";
    }
    std::cout << std::endl;
    return 0;
}



void executeSQL(sqlite3* db, const char* sql, const char* callbackMsg) {
    char* zErrMsg = nullptr;
    int rc = sqlite3_exec(db, sql, callback, (void*)callbackMsg, &zErrMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "SQL error: " << zErrMsg << std::endl;
        sqlite3_free(zErrMsg);
    } else {
        std::cout << callbackMsg << " realizada com sucesso\n";
    }
}



void openDatabase(sqlite3** db) {
    int rc = sqlite3_open("ERPCPP.db", db);
    if (rc) {
        std::cerr << "Não FOI possivel abrir o Banco de Dados: " << sqlite3_errmsg(*db) << std::endl;
    } else {
        std::cout << "Banco de dados aberto com sucesso\n";
    }
}



void closeDatabase(sqlite3* db) {
    sqlite3_close(db);
}



void criarDatabase() {
    sqlite3* db;
    openDatabase(&db);
    closeDatabase(db);
}



void criandoTabela() {
    sqlite3* db;
    openDatabase(&db);

    const char* sql = "CREATE TABLE IF NOT EXISTS COMPANY(" \
                      "ID INT PRIMARY KEY NOT NULL," \
                      "NOME TEXT NOT NULL," \
                      "IDADE INT NOT NULL," \
                      "ENDERECO VARCHAR(60)," \
                      "SALARIO NUMERIC(12,2)," \
                      "HORASBASE NUMERIC(5,2));";
    executeSQL(db, sql, "Tabela criada");

    closeDatabase(db);
}



void insertContratados() {
    sqlite3* db;
    openDatabase(&db);

    int id, idade;
    double salario, horasBase;
    std::string nome, endereco;

    std::cout << "Digite ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Digite Nome: ";
    std::getline(std::cin, nome);
    std::cout << "Digite Idade: ";
    std::cin >> idade;
    std::cin.ignore();
    std::cout << "Digite Endereco: ";
    std::getline(std::cin, endereco);
    std::cout << "Digite Salario: ";
    std::cin >> salario;
    std::cout << "Digite Horas Base: ";
    std::cin >> horasBase;

    std::string sql = "INSERT INTO COMPANY (ID, NOME, IDADE, ENDERECO, SALARIO, HORASBASE) VALUES (" +
                      std::to_string(id) + ", '" + nome + "', " + std::to_string(idade) + ", '" +
                      endereco + "', " + std::to_string(salario) + ", " + std::to_string(horasBase) + ");";

    executeSQL(db, sql.c_str(), "Registro inserido");

    closeDatabase(db);
}



void listaContratados() {
    sqlite3* db;
    openDatabase(&db);

    const char* sql = "SELECT * FROM COMPANY;";
    executeSQL(db, sql, "Lista de contratados");

    closeDatabase(db);
}



void consultaContratadosID() {
    sqlite3* db;
    openDatabase(&db);

    int id;
    std::cout << "Digite o ID: ";
    std::cin >> id;

    std::string sql = "SELECT * FROM COMPANY WHERE ID=" + std::to_string(id) + ";";
    executeSQL(db, sql.c_str(), "Consulta por ID");

    closeDatabase(db);
}



void updateContratados() {
    sqlite3* db;
    openDatabase(&db);

    int id, idade;
    double salario, horasBase;
    std::string nome, endereco;

    std::cout << "Digite ID: ";
    std::cin >> id;
    std::cin.ignore();
    std::cout << "Digite NOVO Nome: ";
    std::getline(std::cin, nome);
    std::cout << "Digite NOVA Idade: ";
    std::cin >> idade;
    std::cin.ignore();
    std::cout << "Digite NOVO Endereço: ";
    std::getline(std::cin, endereco);
    std::cout << "Digite NOVO Salario: ";
    std::cin >> salario;
    std::cout << "Digite NOVAS Horas Base: ";
    std::cin >> horasBase;

    std::string sql = "UPDATE COMPANY SET "
                      "NOME = '" + nome + "', "
                      "IDADE = " + std::to_string(idade) + ", "
                      "ENDERECO = '" + endereco + "', "
                      "SALARIO = " + std::to_string(salario) + ", "
                      "HORASBASE = " + std::to_string(horasBase) +
                      " WHERE ID = " + std::to_string(id) + ";";

    executeSQL(db, sql.c_str(), "Atualizacao Efetuada com Sucesso!!");

    closeDatabase(db);
}


void deleteContratados() {
    sqlite3* db;
    openDatabase(&db);

    int id;
    std::cout << "Digite o ID: ";
    std::cin >> id;

    std::string sql = "DELETE FROM COMPANY WHERE ID = " + std::to_string(id) + ";";
    executeSQL(db, sql.c_str(), "Exclusao do Contratado Efetuado com Sucesso!!");

    closeDatabase(db);
}

void exibirMenu() {
    int opcao;
    do {
        std::cout << "Menu:\n";
        std::cout << "1. Criar Database\n";
        std::cout << "2. Criar Tabela\n";
        std::cout << "3. Inserir Contratado\n";
        std::cout << "4. Listar Contratados\n";
        std::cout << "5. Consultar Contratado por ID\n";
        std::cout << "6. Atualizar Contratado\n";
        std::cout << "7. Deletar Contratado\n";
        std::cout << "8. Sair\n";
        std::cout << "Escolha uma OPCAO: ";
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                criarDatabase();
                break;
            case 2:
                criandoTabela();
                break;
            case 3:
                insertContratados();
                break;
            case 4:
                listaContratados();
                break;
            case 5:
                consultaContratadosID();
                break;
            case 6:
                updateContratados();
                break;
            case 7:
                deleteContratados();
                break;
            case 8:
                std::cout << "Saindo...\n";
                break;
            default:
                std::cout << "Opcao Invalida. Tente Novamente.\n";
        }
    } while (opcao != 8);
}

int main() {
    exibirMenu();
    return 0;
}
