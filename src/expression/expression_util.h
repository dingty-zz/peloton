/*-------------------------------------------------------------------------
 *
 * expression_util.h
 * file description
 *
 * Copyright(c) 2015, CMU
 *
 * /n-store/src/expression/expression_util.h
 *
 *-------------------------------------------------------------------------
 */

#pragma once

#include <string>
#include <vector>

#include "expression/abstract_expression.h"

namespace nstore {
namespace expression {

//===--------------------------------------------------------------------===//
// Expression Utilities
//===--------------------------------------------------------------------===//

// instantiate a typed expression
AbstractExpression* ExpressionFactory(json_spirit::Object &obj,
                                      ExpressionType et, ValueType vt, int vs,
                                      AbstractExpression* lc,
                                      AbstractExpression* rc);


//===--------------------------------------------------------------------===//
// Factories
//===--------------------------------------------------------------------===//

// Several helpers used by expressionFactory() and useful to export to testcases.

AbstractExpression *ComparisonFactory(ExpressionType et, AbstractExpression*, AbstractExpression*);

AbstractExpression *OperatorFactory(ExpressionType et,  AbstractExpression*, AbstractExpression*);

AbstractExpression *ConstantValueFactory(const Value &val);

AbstractExpression *ParameterValueFactory(int idx);

AbstractExpression *TupleValueFactory(int idx);

AbstractExpression *ConjunctionFactory(ExpressionType, AbstractExpression*, AbstractExpression*);

std::string GetTypeName(ExpressionType type);

// If the passed vector contains only TupleValueExpression, it
// returns ColumnIds of them, otherwise NULL.
boost::shared_array<int>
ConvertIfAllTupleValues(const std::vector<AbstractExpression*> &expressions);

// If the passed vector contains only ParameterValueExpression, it
// returns ParamIds of them, otherwise NULL.
boost::shared_array<int>
ConvertIfAllParameterValues(const std::vector<AbstractExpression*> &expressions);

} // End expression namespace
} // End nstore namespace
