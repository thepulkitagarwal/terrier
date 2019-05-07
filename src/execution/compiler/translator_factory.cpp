#include <execution/util/macros.h>
#include <execution/compiler/pipeline.h>
#include "execution/compiler/translator_factory.h"
#include "execution/compiler/operator/aggregate_translator.h"
#include "execution/compiler/operator/seq_scan_translator.h"
#include "execution/compiler/expression/arithmetic_translator.h"
#include "execution/compiler/expression/comparison_translator.h"
#include "execution/compiler/expression/constant_translator.h"
#include "execution/compiler/expression/conjunction_translator.h"
#include "execution/compiler/expression/unary_translator.h"

namespace tpl::compiler {

OperatorTranslator *TranslatorFactory::CreateTranslator(const terrier::planner::AbstractPlanNode &op, Pipeline *pipeline) {
    switch (op.GetPlanNodeType()) {
      case terrier::planner::PlanNodeType::SEQSCAN: {
        return new (pipeline->GetRegion()) SeqScanTranslator(planNode, context);
      }
      case terrier::planner::PlanNodeType::AGGREGATE: {
        return new(context.GetRegion()) AggregateTranslator(planNode, context);
      }
      default:
        TPL_ASSERT(false, "Unsupported plan node for translation");
    }
  }

ExpressionTranslator *TranslatorFactory::CreateTranslator(const terrier::expression::AbstractExpression *expression, CompilationContext &context) {
  auto type = expression->GetExpressionType();
  if(COMPARISON_OP(type)){
    auto ret = new (context.GetRegion()) ComparisonTranslator(expression, context);
    return reinterpret_cast<ExpressionTranslator*>(ret);
  }
  if(ARITHMETIC_OP(type)){
    auto ret = new (context.GetRegion()) ArithmeticTranslator(expression, context);
    return reinterpret_cast<ExpressionTranslator*>(ret);
  }
  if(UNARY_OP(type)){
    auto ret = new (context.GetRegion()) UnaryTranslator(expression, context);
    return reinterpret_cast<ExpressionTranslator*>(ret);
  }
  if(CONJUNCTION_OP(type)){
    auto ret = new (context.GetRegion()) ConjunctionTranslator(expression, context);
    return reinterpret_cast<ExpressionTranslator*>(ret);
  }
  if(CONSTANT_VAL(type)){
    auto ret = new (context.GetRegion()) ConstantTranslator(expression, context);
    return reinterpret_cast<ExpressionTranslator*>(ret);
  }


}

} //namespace tpl::compiler