#include "planner/plannodes/output_schema.h"

namespace tpl::exec {
/**
 * Used by the tpl executable to retrieve test output schemas
 */
class SampleOutput {
 public:
  SampleOutput() = default;

  void InitTestOutput() {
    // Sample output formats:
    terrier::planner::OutputSchema::Column int_col{"dummy_name", terrier::type::TypeId::INTEGER, true, terrier::catalog::col_oid_t{0}};
    // Create schemas with up to 10 integer columns.
    for (int i = 0; i < 10; i++) {
      std::vector<terrier::planner::OutputSchema::Column> cols;
      for (int j = 0; j < i + 1; j++) {
        cols.emplace_back(int_col);
      }
      schemas_.emplace("schema" + std::to_string(i+1), terrier::planner::OutputSchema(cols));
    }
  }

  const terrier::planner::OutputSchema * GetSchema(const std::string & name) {
    return &schemas_.at(name);
  }

 private:
  std::unordered_map<std::string, terrier::planner::OutputSchema> schemas_;
};
}