
#include "../include/Recorder.hpp"
#include <iostream>
#include <utility>
void Recorder::add(int line, Statement *stmt)
{
    if (record.count(line))
    {
        record[line] = stmt;
    }
    else
    {
        record.insert(std::make_pair(line, stmt));
    }
    max_PC = std::max(max_PC, line);
}

void Recorder::remove(int line)
{
    if (record.count(line))
    {
        record.erase(record.find(line));
    }
    if (max_PC == line)
    {
        int i = line - 1;
        while (i > 0 && !record.count(i))
        {
            i--;
        }
        max_PC = i;
    }
}

const Statement *Recorder::get(int line) const noexcept
{
    if (record.count(line))
    {
        return (record.find(line))->second;
    }
    else
    {
        return nullptr;
    }
}

bool Recorder::hasLine(int line) const noexcept
{
    if (record.count(line))
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Recorder::clear() noexcept
{
    record.clear();
    max_PC = 0;
}

void Recorder::printLines() const
{
    for (auto i = record.begin(); i != record.end(); i++)
    {
        std::string message = (i->second)->text();
        std::cout << message << '\n';
    }
}
int Recorder::nextLine(int line) const noexcept
{
    for (int i = line + 1; i <= max_PC; i++)
    {
        if (record.count(i))
        {
            return i;
        }
    }
    return -1;
}
