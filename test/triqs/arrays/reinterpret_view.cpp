/*******************************************************************************
 *
 * TRIQS: a Toolbox for Research in Interacting Quantum Systems
 *
 * Copyright (C) 2013 by O. Parcollet
 *
 * TRIQS is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * TRIQS is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * TRIQS. If not, see <http://www.gnu.org/licenses/>.
 *
 ******************************************************************************/

#include <triqs/arrays.hpp>
#include <iostream>

namespace triqs { namespace arrays { 

/* template <typename V, int R, ull_t Opt, ull_t To, typename ... I>  
  array_view<V, sizeof...(I), Opt> reinterpret_array_view (array<V,R,Opt,To> const & a, I ... index) { 
   //static int constexpr rank = sizeof...(I);
   typedef array_view<V, sizeof...(I), Opt, indexmaps::mem_layout::c_order(rank)> return_t;
   //typedef array_view<V, rank, Opt, indexmaps::mem_layout::c_order(rank)> return_t;
   return return_t { make_shape(index...), a.storage() };
   //return return_t (typename return_t::indexmap_type (mini_vector<size_t,rank>(index...)) , a.storage());
  }
*/

 template <typename V, int R, ull_t Opt, ull_t To, typename ... I>  
  array_view<V, sizeof...(I), Opt> reinterpret (array<V,R,Opt,To> const & a, I ... index) { 
   return { {make_shape(index...)}, a.storage() };
  }

 // wrong for views
 template <typename V, int R, ull_t Opt, ull_t To, bool B, typename ... I>  
  array_view<V, sizeof...(I), Opt,indexmaps::mem_layout::c_order(sizeof...(I))> reinterpret_array_view (array_view<V,R,Opt,To,B> const & a, I ... index) { 
   if (!has_contiguous_data(a)) TRIQS_RUNTIME_ERROR << "reinterpretation failure : data of the view are not contiguous";
   return { {make_shape(index...)}, a.storage() };
  }
/*
 template <typename V, int R, ull_t Opt, ull_t To, typename ... I>  
  array_view<V, sizeof...(I), Opt,indexmaps::mem_layout::c_order(sizeof...(I))> reinterpret_array_view_c (matrix<V,Opt,To> const & a, I ... index) { 
   return { {make_shape(index...)}, a.storage() };
  }

template <typename V, int R, ull_t Opt, ull_t To, typename ... I>  
  array_view<V, sizeof...(I), Opt,indexmaps::mem_layout::c_order(sizeof...(I))> reinterpret_array_view_c (vector<V,Opt> const & a, I ... index) { 
   return { {make_shape(index...)}, a.storage() };
  }
*/

}}

using namespace triqs::arrays;
int main(int argc, char **argv) {
 triqs::arrays::array<long,1> A = {1,2,3,4,5,6};
 std::cout << reinterpret(A, 2,3)<< std::endl;
 //std::cout << reinterpret_array_view(A(range(1,5)), 2,3)<< std::endl;
 return 0;
}





