/**
* @version		GrPPI v0.2
* @copyright		Copyright (C) 2017 Universidad Carlos III de Madrid. All rights reserved.
* @license		GNU/GPL, see LICENSE.txt
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You have received a copy of the GNU General Public License in LICENSE.txt
* also available in <http://www.gnu.org/licenses/gpl.html>.
*
* See COPYRIGHT.txt for copyright notices and details.
*/


#ifndef GRPPI_OMP_STREAM_REDUCE_H
#define GRPPI_OMP_STREAM_REDUCE_H

#ifdef GRPPI_OMP

namespace grppi{

template <typename Generator, typename Combiner, typename Consumer, 
          typename Identity>
void stream_reduce(parallel_execution_omp &ex, Generator generate_op, 
                   int window_size, int offset, 
                   Combiner && combine_op, Consumer consume_op, 
                   Identity identity)
{
  using namespace std;
  using generated_type = typename result_of<Generator()>::type;
  using generated_value_type = typename generated_type::value_type;
  // TODO: Evaluate better structure than vector
  vector<generated_value_type> values;
  values.reserve(window_size);

  // TODO: Set generator and consumer in separate threads
  auto item = generate_op();
  for (;;) {
    while (item && values.size()!=window_size) {
      values.push_back(*item);
      item = generate_op();
    }
    if (values.size()>0) {
      auto reduced_value = reduce(ex, values.begin(), values.end(), identity,
          std::forward<Combiner>(combine_op));
      consume_op(reduced_value);
      if (item) {
        values.erase(values.begin(), values.begin() + offset);
      }
    }
    if (!item) break;
  }
}

}

#endif

#endif
