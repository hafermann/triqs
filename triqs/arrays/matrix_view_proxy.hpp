/*******************************************************************************
 *
 * TRIQS: a Toolbox for Research in Interacting Quantum Systems
 *
 * Copyright (C) 2011 by O. Parcollet
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
#ifndef TRIQS_ARRAYS_MATRIX_VIEW_PROXY_H
#define TRIQS_ARRAYS_MATRIX_VIEW_PROXY_H
#include <boost/preprocessor/repetition/enum.hpp>
#include <boost/preprocessor/repetition/enum_trailing.hpp>
#include <boost/preprocessor/repetition/repeat.hpp>

namespace triqs { namespace arrays {

 template<typename ArrayType,int Pos > class matrix_view_proxy;
 template<typename ArrayType,int Pos > class const_matrix_view_proxy;

#define AUX0(z,P,NNN) std::forward<A##P>(a##P),
#define AUX1(z,P,NNN) A##P && a##P,
#define TEXT(z, n, text) text
#define IMPL(z, POS, unused)\
 template<typename ArrayType > class const_matrix_view_proxy<ArrayType,POS> : TRIQS_CONCEPT_TAG_NAME(ImmutableMatrix)   {\
   static_assert(ArrayType::rank==3, "Only array of rank 3 here");\
   ArrayType const * A; size_t n;\
  public :\
   typedef typename ArrayType::value_type value_type;\
   const_matrix_view_proxy (ArrayType const &  A_, size_t n_=0) : A(&A_), n(n_){}\
   typedef indexmaps::slicer<typename ArrayType::indexmap_type BOOST_PP_ENUM_TRAILING(POS, TEXT, range),size_t,ellipsis> slicer_t;\
   typedef typename slicer_t::r_type indexmap_type;\
   typedef typename indexmap_type::domain_type domain_type;\
   indexmap_type indexmap() const { return slicer_t::invoke(A->indexmap() BOOST_PP_ENUM_TRAILING(POS, TEXT, range()),n, ellipsis()); }\
   domain_type domain() const { return indexmap().domain();}\
   friend size_t get_shape (const_matrix_view_proxy const & x) { return get_shape(*x.A);}\
   friend size_t first_dim (const_matrix_view_proxy const & x) { return get_shape(*x.A)[(POS+1)%3];}\
   friend size_t second_dim(const_matrix_view_proxy const & x) { return get_shape(*x.A)[(POS+2)%3];}\
   typename ArrayType::storage_type const & storage() const { return A->storage();}\
   value_type const * restrict data_start() const { return &storage()[indexmap().start_shift()];}\
   value_type * restrict data_start() { return &storage()[indexmap().start_shift()];}\
   matrix_view<value_type> operator()() const {return *this;}\
   TRIQS_DELETE_COMPOUND_OPERATORS(const_matrix_view_proxy);\
  template<BOOST_PP_ENUM_PARAMS(POS,typename A) BOOST_PP_COMMA_IF(POS) typename ... Args>\
  value_type const & operator() (BOOST_PP_REPEAT(POS,AUX1,nil)  Args && ... args) const \
  { return (*A)(BOOST_PP_REPEAT(POS,AUX0,nil) n,std::forward<Args>(args)...);}\
  friend std::ostream & operator <<(std::ostream & out, const_matrix_view_proxy const & x) { return out << matrix_view<value_type>(x);}\
 };\
\
 template<typename ArrayType > class matrix_view_proxy<ArrayType,POS> : TRIQS_CONCEPT_TAG_NAME(MutableMatrix)   {\
   static_assert(ArrayType::rank==3, "Only array of rank 3 here");\
   ArrayType * A; size_t n;\
  public :\
   typedef typename ArrayType::value_type value_type;\
   matrix_view_proxy (ArrayType &  A_, size_t n_=0) : A(&A_), n(n_){}\
   typedef indexmaps::slicer<typename ArrayType::indexmap_type BOOST_PP_ENUM_TRAILING(POS, TEXT, range),size_t,ellipsis> slicer_t;\
   typedef typename slicer_t::r_type indexmap_type;\
   typedef typename indexmap_type::domain_type domain_type;\
  indexmap_type indexmap() const { return slicer_t::invoke(A->indexmap() BOOST_PP_ENUM_TRAILING(POS, TEXT, range()),n, ellipsis()); }\
   domain_type domain() const { return indexmap().domain();}\
   friend size_t get_shape (matrix_view_proxy const & x) { return get_shape(*x.A);}\
   friend size_t first_dim (matrix_view_proxy const & x) { return get_shape(*x.A)[(POS+1)%3];}\
   friend size_t second_dim(matrix_view_proxy const & x) { return get_shape(*x.A)[(POS+2)%3];}\
   typename ArrayType::storage_type const & storage() const { return A->storage();}\
   value_type const * restrict data_start() const { return &storage()[indexmap().start_shift()];}\
   value_type * restrict data_start() { return &storage()[indexmap().start_shift()];}\
   matrix_view<value_type> operator()() const {return *this;}\
   template<typename RHS> matrix_view_proxy & operator=(const RHS & X) {triqs_arrays_assign_delegation(*this,X); return *this; }\
   TRIQS_DEFINE_COMPOUND_OPERATORS(matrix_view_proxy);\
   template<BOOST_PP_ENUM_PARAMS(POS,typename A) BOOST_PP_COMMA_IF(POS) typename ... Args>\
   value_type & operator() (BOOST_PP_REPEAT(POS,AUX1,nil) Args && ... args) const\
   { return (*A)(BOOST_PP_REPEAT(POS,AUX0,nil) n,std::forward<Args>(args)...);}\
   friend std::ostream & operator <<(std::ostream & out, matrix_view_proxy const & x) { return out << matrix_view<value_type>(x);}\
};

 BOOST_PP_REPEAT(ARRAY_NRANK_MAX , IMPL, nil);
#undef IMPL
#undef AUX0
#undef AUX1
#undef TEXT

}}
#endif

