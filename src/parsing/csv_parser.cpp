#include <cstring>
#include <fstream>
#include <regex>
#include <sstream>

#include "csv_parser.h"

csv_parser::csv_parser(std::string path) {
    this->file = std::ifstream(path);
}

bool csv_parser::is_open() {
    return this->file.is_open();
}

bool csv_parser::read_line() {
    // lê a próxima linha do .csv. Caso a linha não esteja totalmente carregada
    // na memória, a parte inicial da linha já presente na memória principal (se
    // houver) é movida para o início do buffer e uma nova leitura de disco é
    // executada para completar o buffer
    if (!this->read_buffer_line()) {
        if (this->file.eof()) {
            return false;
        }

        size_t remainder = this->characters_read;
        if (remainder > 0) {
            strncpy(this->buffer.data(), &this->buffer[buffer_size - remainder],
                    remainder);
        }

        file.read(&this->buffer[remainder], this->buffer.size() - remainder);
        this->buffer_size = remainder + this->file.gcount();
        this->index = 0;

        this->read_buffer_line();
    }

    return true;
}

bool csv_parser::read_buffer_line() {
    this->columns.clear();

    int j = this->index;
    bool in_quotes = false;

    int i;
    for (i = this->index; i < this->buffer_size; i++) {
        if (in_quotes && this->buffer[i] == '"') {
            if (i + 1 < this->buffer.size() && this->buffer[i + 1] == '"') {
                // pular aspas duplas escapadas "", que pertencem ao próprio
                // conteúdo da coluna
                i += 1;
            } else {
                // as aspas são aspas de fechamento do conteúdo da coluna
                in_quotes = false;
            }
        } else if (!in_quotes) {
            if (this->buffer[i] == '"') {
                // abertura de aspas do conteúdo da coluna
                in_quotes = true;
            } else if (this->buffer[i] == ',' || this->buffer[i] == '\n') {
                // , ou \n fora de aspas => fim de uma coluna

                size_t position = j;
                size_t length = i - j;

                // remove as aspas externas do conteúdo da coluna
                if (i > 0) {
                    // caso as linhas sejam terminadas com CRLF (Windows)
                    if (this->buffer[i - 1] == '\r') {
                        if (this->buffer[i - 2] == '"') {
                            position += 1;
                            length -= 3;
                        }
                    } else if (this->buffer[i - 1] == '"') {
                        position += 1;
                        length -= 2;
                    }
                }

                this->columns.push_back({position, length});

                // posição da próxima coluna
                j = i + 1;

                if (this->buffer[i] == '\n') {
                    this->characters_read = (i + 1) - this->index;
                    this->index = (i + 1);

                    return true;
                }
            }
        }
    }

    this->characters_read = i - this->index;
    this->index = i;

    return false;
}

template <>
bool csv_parser::get<std::string>(size_t index, std::string &out) {
    if (index >= this->columns.size()) {
        return false;
    }

    csv_column column = this->columns[index];
    if (column.length == 0) {
        return false;
    }

    out = std::string(&this->buffer[column.position], column.length);

    return true;
}

template <>
bool csv_parser::get<char>(size_t index, char &out) {
    return this->get(index, out, "%c");
}

template <>
bool csv_parser::get<unsigned char>(size_t index, unsigned char &out) {
    return this->get(index, out, "%hhu");
}

template <>
bool csv_parser::get<short>(size_t index, short &out) {
    return this->get(index, out, "%hd");
}

template <>
bool csv_parser::get<unsigned short>(size_t index, unsigned short &out) {
    return this->get(index, out, "%hu");
}

template <>
bool csv_parser::get<int>(size_t index, int &out) {
    return this->get(index, out, "%d");
}

template <>
bool csv_parser::get<unsigned int>(size_t index, unsigned int &out) {
    return this->get(index, out, "%u");
}

template <>
bool csv_parser::get<long>(size_t index, long &out) {
    return this->get(index, out, "%ld");
}

template <>
bool csv_parser::get<unsigned long>(size_t index, unsigned long &out) {
    return this->get(index, out, "%lu");
}

template <>
bool csv_parser::get<float>(size_t index, float &out) {
    return this->get(index, out, "%f");
}

template <>
bool csv_parser::get<double>(size_t index, double &out) {
    return this->get(index, out, "%lf");
}

template <>
bool csv_parser::get<std::vector<int>>(size_t index, std::vector<int> &out) {
    // lê a coluna como string
    std::string s;
    if (!this->get(index, s)) {
        return false;
    }

    // filtra os caracteres '[', ']' e ','
    std::istringstream ss(std::regex_replace(s, std::regex{R"(\[|\]|,)"}, " "));
    std::string token;
    while (ss >> token) {
        try {
            out.push_back(std::stoi(token));
        } catch (...) {
            continue;
        }
    }

    return true;
}