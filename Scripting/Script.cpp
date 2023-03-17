#include "Script.h"
#include "CFlat.h"
#include "FlowManager.h"
#include <iostream>
void Script::iteration(CFlat::IBox* iteration, std::ofstream& stream)
{
    while (iteration != nullptr)
    {
        iteration->processBox(stream);
        iteration = iteration->nextBox;
    }
}

void Script::setupScript(std::string script)
{
    CFlat::FlowManager::instance->loadScript(script, this);
}

void Script::init(std::ofstream& stream)
{
    if (!initBox)
        return;

    iteration(initBox, stream);
}

void Script::update(std::ofstream& stream)
{
    if (!updateBox)
        return;
    iteration(updateBox, stream);
}
