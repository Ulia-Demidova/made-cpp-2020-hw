#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H

#include "graph.h"
#include "type_list.h"
#include "value_list.h"

template <Type flag, typename Graph, size_t start, size_t end,
          typename... Edges>
struct DFS {
  using result = std::false_type; // flag == true means we shouldn't start DFS
                                  // and just return false
};

template <typename Graph, size_t start, size_t end, Type... visitors,
          typename... Edges>
struct DFS<false, Graph, start, end, ValueList<visitors...>,
           TypeList<Edges...>> {
private:
  using used = typename Set<start, true, ValueList<visitors...>>::type;
  using Head_Edge = typename TypeList<Edges...>::Head;
  using Tail_Edges = typename TypeList<Edges...>::Tail;

  static constexpr Type next_is_visited = Get<Head_Edge::to, used>::value;

public:
  using head_result =
      typename DFS<next_is_visited, Graph, Head_Edge::to, end, used,
                   typename TypeAt<Head_Edge::to, typename Graph::Vertices>::
                       type::Edges>::result;
  using tail_result =
      typename DFS<false, Graph, start, end, used, Tail_Edges>::result;

  using result = typename std::disjunction<head_result, tail_result>::type;
};

template <typename Graph, size_t start, size_t end, Type... visitors>
struct DFS<false, Graph, start, end, ValueList<visitors...>, NullType> {
  using result = std::false_type;
};

template <typename Graph, size_t start, Type... visitors>
struct DFS<false, Graph, start, start, ValueList<visitors...>, NullType> {
  using result = std::true_type;
};

template <typename Graph, size_t start, Type... visitors, typename... Edges>
struct DFS<false, Graph, start, start, ValueList<visitors...>,
           TypeList<Edges...>> {
  using result = std::true_type;
};

template <typename Graph, size_t start, size_t end> struct PathExists {
private:
  using used =
      typename Construct<false, Length<typename Graph::Vertices>::value>::type;
  using edges = typename TypeAt<start, typename Graph::Vertices>::type::Edges;
  using result = typename DFS<false, Graph, start, end, used, edges>::result;

public:
  static constexpr bool value = result::value;
};

#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_THIRD_VARIANT_DFS_H.
