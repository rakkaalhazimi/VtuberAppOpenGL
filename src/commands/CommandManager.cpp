#include "commands/CommandManager.h"


void CommandManager::executeCommand(std::unique_ptr<Command> command)
{
  command->execute();
  undoStack.push(std::move(command));
}

void CommandManager::undo()
{
  if (!undoStack.empty())
  {
    undoStack.top()->undo();
    undoStack.pop();
  }
}