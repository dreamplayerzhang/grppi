/*
 * Copyright 2018 Universidad Carlos III de Madrid
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef GRPPI_STENCIL_H 
#define GRPPI_STENCIL_H

#include <tuple>
#include <utility>

#include "common/execution_traits.h"
#include "common/iterator_traits.h"

namespace grppi {

/**
\addtogroup data_patterns
@{
\defgroup stencil_pattern Stencil pattern
\brief Interface for applyinng the \ref md_stencil.
@{
*/

/**
\brief Invoke \ref md_stencil on a data sequence with
sequential execution.
\tparam Execution Execution type.
\tparam InputIterators Iterators types used for the input sequences.
\tparam OutputIt Iterator type used for the output sequence
\tparam StencilTransformer Callable type for performing the stencil transformation.
\tparam Neighbourhood Callable type for obtaining the neighbourhood.
\param ex Execution policy object.
\param firsts Tuple of iterator to the first elements of the input sequences.
\param size Size of the input sequence to be proccess.
\param out Iterator to the first element in the output sequence.
\param transform_op Stencil transformation operation.
\param neighbour_op Neighbourhood operation.
*/
template <typename Execution, typename ...InputIterators, typename OutputIt,
          typename StencilTransformer, typename Neighbourhood,
          requires_iterators<InputIterators...> = 0,
          requires_iterator<OutputIt> = 0>
void stencil(
    const Execution & ex,
    std::tuple<InputIterators...> firsts, std::size_t size, OutputIt out,
    StencilTransformer && transform_op,
    Neighbourhood && neighbour_op)
{
  static_assert(supports_stencil<Execution>(),
                "stencil not supported for execution type");
  ex.stencil(firsts, out, size,
             std::forward<StencilTransformer>(transform_op),
             std::forward<Neighbourhood>(neighbour_op));
}

/**
\brief Invoke \ref md_stencil on a data sequence with
sequential execution.
\tparam Execution Execution type.
\tparam InputIterators Iterators types used for the input sequences.
\tparam InputIt Iterator type used for the input sequence.
\tparam OutputIt Iterator type used for the output sequence
\tparam StencilTransformer Callable type for performing the stencil transformation.
\tparam Neighbourhood Callable type for obtaining the neighbourhood.
\param ex Execution policy object.
\param firsts Tuple of iterator to the first elements of the input sequences.
\param last Iterator to one past the end of the input sequence.
\param out Iterator to the first element in the output sequence.
\param transform_op Stencil transformation operation.
\param neighbour_op Neighbourhood operation.
*/
template <typename Execution, typename ...InputIterators,
          typename InputIt, typename OutputIt,
          typename StencilTransformer, typename Neighbourhood,
          requires_iterators<InputIterators...> = 0,
          requires_iterator<InputIt> = 0,
          requires_iterator<OutputIt> = 0>
void stencil(
    const Execution & ex,
    std::tuple<InputIterators...> firsts, InputIt last, OutputIt out,
    StencilTransformer && transform_op,
    Neighbourhood && neighbour_op)
{
  static_assert(supports_stencil<Execution>(),
                "stencil not supported for execution type");
  ex.stencil(firsts, out,
             std::distance(std::get<0>(firsts),last),
             std::forward<StencilTransformer>(transform_op),
             std::forward<Neighbourhood>(neighbour_op));
}

/**
\brief Invoke \ref md_stencil on a data sequence with 
sequential execution.
\tparam Execution Execution type.
\tparam InputIt Iterator type used for the input sequence.
\tparam OutputIt Iterator type used for the output sequence
\tparam Neighbourhood Callable type for obtaining the neighbourhood.
\tparam StencilTransformer Callable type for performing the stencil transformation.
\param ex Execution policy object.
\param first Iterator to the first element in the input sequence.
\param last Iterator to one past the end of the input sequence.
\param out Iterator to the first element in the output sequence.
\param transform_op Stencil transformation operation.
\param neighbour_op Neighbourhood operation.
*/
template <typename Execution, typename InputIt, typename OutputIt, 
          typename StencilTransformer, typename Neighbourhood,
          requires_iterator<InputIt> = 0,
          requires_iterator<OutputIt> = 0>
void stencil(
    const Execution & ex, 
    InputIt first, InputIt last, OutputIt out, 
    StencilTransformer && transform_op, 
    Neighbourhood && neighbour_op) 
{
  static_assert(supports_stencil<Execution>(),
      "stencil not supported for execution type");
  ex.stencil(std::make_tuple(first), out,
      std::distance(first,last),
      std::forward<StencilTransformer>(transform_op),
      std::forward<Neighbourhood>(neighbour_op));
}

/**
\brief Invoke \ref md_stencil on multiple data sequences with 
sequential execution.
\tparam Execution Execution type.
\tparam InputIt Iterator type used for the input sequence.
\tparam OutputIt Iterator type used for the output sequence
\tparam Neighbourhood Callable type for obtaining the neighbourhood.
\tparam StencilTransformer Callable type for performing the stencil transformation.
\tparam OtherInputIts Iterator types for additional input sequences.
\param ex Execution policy object.
\param first Iterator to the first element in the input sequence.
\param last Iterator to one past the end of the input sequence.
\param out Iterator to the first element in the output sequence.
\param transform_op Stencil transformation operation.
\param neighbour_op Neighbourhood operation.
\param other_firsts Iterators to the first element of additional input sequences.
*/
template <typename Execution, typename InputIt, typename OutputIt, 
          typename StencilTransformer, typename Neighbourhood, 
          typename ... OtherInputIts,
          requires_iterator<InputIt> = 0,
          requires_iterator<OutputIt> = 0>
[[deprecated("This version of the interface is deprecated.\n"
             "If you want to use multiple inputs, use a tuple instead.")]]
void stencil(
    const Execution & ex, 
    InputIt first, InputIt last, OutputIt out, 
    StencilTransformer && transform_op, 
    Neighbourhood && neighbour_op, 
    OtherInputIts ... other_firsts) 
{
  static_assert(supports_stencil<Execution>(),
      "stencil not supported for execution type");
  ex.stencil(std::make_tuple(first,other_firsts...), out,
      std::distance(first,last),
      std::forward<StencilTransformer>(transform_op),
      std::forward<Neighbourhood>(neighbour_op));
}

/**
@}
@}
*/

}

#endif
