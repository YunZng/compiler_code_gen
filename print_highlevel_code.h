#ifndef PRINT_HIGHLEVEL_CODE_H
#define PRINT_HIGHLEVEL_CODE_H

#include "module_collector.h"

// Implementation of ModuleCollector that just prints high level
// code to stdout for informational/debugging purposes.
class PrintHighlevelCode : public ModuleCollector {
private:
  enum Mode {
    NONE, RODATA, DATA, CODE,
  };
  Mode m_mode;

public:
  PrintHighlevelCode();
  virtual ~PrintHighlevelCode();

  virtual void collect_string_constant(const std::string &name, const std::string &strval);
  virtual void collect_global_var(const std::string &name, const std::shared_ptr<Type> &type);
  virtual void collect_function(const std::string &name, const std::shared_ptr<InstructionSequence> &iseq);

private:
  void set_mode(Mode mode);
};

#endif // PRINT_HIGHLEVEL_CODE_H
