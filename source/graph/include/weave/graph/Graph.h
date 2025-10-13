// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPH_H_2025_10_05_14_42_23
#define GRAPH_H_2025_10_05_14_42_23

#include <weave/graph/Node.h>
#include <weave/graph/Edge.h>
#include <weave/user/BufferData.h>
#include <weave/user/Module.h>

namespace weave
{
	namespace graph
	{
		template<typename GraphDescriptorType>
		struct GraphDescriptionToTuples;
		template<typename... NodeDescriptorTypes, typename... EdgeDescriptorTypes>
		struct GraphDescriptionToTuples<GraphDescriptor<NodeDescriptorList<NodeDescriptorTypes...>, EdgeDescriptorList<EdgeDescriptorTypes...> > >
		{
			using NodeTuple = std::tuple<Node<NodeDescriptorTypes>...>;
			using NodeContextTuple = std::tuple<typename user::Module<typename NodeDescriptorTypes::Tag>::ContextType...>;
			using EdgeTuple = std::tuple<Edge<EdgeDescriptorTypes>...>;
			using EdgeContextTuple = std::tuple<typename user::BufferData<typename EdgeDescriptorTypes::Tag>::ContextType...>;
		};

		template<typename NodeType>
		struct ExtractDescriptorParamsFromNode;
		template<typename NodeTag, uint64_t numInputs, uint64_t numOutputs>
		struct ExtractDescriptorParamsFromNode<Node<NodeDescriptor<NodeTag, numInputs, numOutputs> > >
		{
			using Tag = NodeTag;
			static constexpr uint64_t inputs = numInputs;
			static constexpr uint64_t outputs = numOutputs;
		};

		template<typename NodeType>
		struct ExtractDescriptorParamsFromEdge;
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag>
		struct ExtractDescriptorParamsFromEdge<Edge<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag> > >
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
		};

		template<typename T>
		struct ShowType;

		template<typename GraphDescriptorType>
		class Graph
		{
		public:
			using NodeTuple = typename GraphDescriptionToTuples<GraphDescriptorType>::NodeTuple;
			using NodeContextTuple = typename GraphDescriptionToTuples<GraphDescriptorType>::NodeContextTuple;
			using EdgeTuple = typename GraphDescriptionToTuples<GraphDescriptorType>::EdgeTuple;
			using EdgeContextTuple = typename GraphDescriptionToTuples<GraphDescriptorType>::EdgeContextTuple;
			explicit Graph(NodeContextTuple nodeContexts, EdgeContextTuple edgeContexts) :
				_nodes(_constructTuple<NodeTuple>(nodeContexts, std::make_index_sequence<std::tuple_size_v<NodeContextTuple> >())),
				_edges(_constructTuple<EdgeTuple>(edgeContexts, std::make_index_sequence<std::tuple_size_v<EdgeContextTuple> >()))
			{}

			void start()
			{
				_startEdges(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >());
				_startNodes(std::make_index_sequence<std::tuple_size_v<NodeTuple> >());
			}

		private:
			// Deduction can only happen from function arguments (end of the list), the previous template arguments have to be explicit (TupleType)
			template<typename TupleType, typename ContextTupleType, std::size_t... Indices>
			TupleType _constructTuple(ContextTupleType& contexts, std::index_sequence<Indices...> sequence)
			{
				// We pass a dummy object to deduce the indices (not the nicest, but seems to be used a lot). Passing it as template parameter requires too many helper constructs.
				TupleType tuple = {std::tuple_element_t<Indices, TupleType>(std::get<Indices>(contexts))...};
				return tuple;
			}

			template<std::size_t... EdgeIndices>
			void _startEdges(std::index_sequence<EdgeIndices...> sequence)
			{
				(std::get<EdgeIndices>(_edges).start(), ...);
			}

			template<std::size_t... NodeIndices>
			void _startNodes(std::index_sequence<NodeIndices...> sequence)
			{
				(_startNode<NodeIndices>(), ...);
			}

			template<std::size_t NodeIndex>
			void _startNode()
			{
				auto inEdges = _getInEdges<NodeIndex>(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >()); // Dummy to infer edge indeces
				auto outEdges = _getOutEdges<NodeIndex>(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >()); // Idem.
				std::get<NodeIndex>(_nodes).start(inEdges, outEdges);
			}

			template<std::size_t NodeIndex, std::size_t... EdgeIndeces>
			auto _getInEdges(std::index_sequence<EdgeIndeces...> sequence)
			{
				auto inEdgesTuple = std::tuple_cat(_conditionalInEdgeTuple<NodeIndex, EdgeIndeces>()...);
				return inEdgesTuple;
			}

			template<std::size_t NodeIndex, std::size_t... EdgeIndeces>
			auto _getOutEdges(std::index_sequence<EdgeIndeces...> sequence)
			{
				auto outEdgesTuple = std::tuple_cat(_conditionalOutEdgeTuple<NodeIndex, EdgeIndeces>()...);
				return outEdgesTuple;
			}

			template<std::size_t NodeIndex, std::size_t EdgeIndex>
			auto _conditionalInEdgeTuple()
			{
				using NodeType = std::tuple_element_t<NodeIndex, NodeTuple>;
				using NodeTag = typename ExtractDescriptorParamsFromNode<NodeType>::Tag;

				using EdgeType = std::tuple_element_t<EdgeIndex, EdgeTuple>;
				using ToNodeTag = typename ExtractDescriptorParamsFromEdge<EdgeType>::ToNode;

				return _conditionalTuple<NodeTag, ToNodeTag, EdgeIndex>();
			}

			template<std::size_t NodeIndex, std::size_t EdgeIndex>
			auto _conditionalOutEdgeTuple()
			{
				using NodeType = std::tuple_element_t<NodeIndex, NodeTuple>;
				using NodeTag = typename ExtractDescriptorParamsFromNode<NodeType>::Tag;

				using EdgeType = std::tuple_element_t<EdgeIndex, EdgeTuple>;
				using FromNodeTag = typename ExtractDescriptorParamsFromEdge<EdgeType>::FromNode;

				return _conditionalTuple<NodeTag, FromNodeTag, EdgeIndex>();
			}

			template<typename NodeTagTypeLeft, typename NodeTagTypeRight, std::size_t EdgeIndex>
			auto _conditionalTuple()
			{
				using EdgeType = std::tuple_element_t<EdgeIndex, EdgeTuple>;
				if constexpr (std::is_same_v<NodeTagTypeLeft, NodeTagTypeRight>)
				{
					std::tuple<EdgeType&> tuple = std::tuple<EdgeType&>(std::get<EdgeIndex>(_edges)); // std::make_tuple may drop references/const (decay behavior) depending on implementation
					return tuple;
				}
				else
				{
					std::tuple<> tuple;
					return tuple;
				}
			}

			NodeTuple _nodes;
			EdgeTuple _edges;
		};
	}
}

#endif

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
		template<typename GraphDescriptorType>
		class Graph
		{
		public:
			using NodeContextTuple = typename GraphDescriptorToContextTuples<GraphDescriptorType>::NodeContextTuple;
			using EdgeContextTuple = typename GraphDescriptorToContextTuples<GraphDescriptorType>::EdgeContextTuple;
			using NodesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::NodesTuple;
			using EdgesTuple = typename GraphDescriptorToGraphTuples<GraphDescriptorType>::EdgesTuple;

			Graph(NodeContextTuple& nodeContexts, EdgeContextTuple& edgeContexts) : _nodes(_constructTuple<NodesTuple>(nodeContexts, std::make_index_sequence<std::tuple_size_v<NodesTuple> >{})),
			                                                                        _edges(_constructTuple<EdgesTuple>(edgeContexts, std::make_index_sequence<std::tuple_size_v<EdgesTuple> >{}))
			{}
			void start()
			{
				_startNodes(std::make_index_sequence<std::tuple_size_v<NodesTuple> >{}); // Arguments: 0, 1, 2, ...
			}

		private:
			template<std::size_t... NodeIndices>
			void _startNodes(std::index_sequence<NodeIndices...>)
			{
				(_startNodeAtIndex<NodeIndices>(), ...);
			}

			template<std::size_t NodeIndex, std::size_t EdgeIndex>
			auto _conditionalInEdges()
			{
				using NodeType = std::tuple_element_t<NodeIndex, NodesTuple>;
				using EdgeType = std::tuple_element_t<EdgeIndex, EdgesTuple>;

				if constexpr (EdgeFromNode<EdgeType, NodeType>::value)
				{
					return std::make_tuple(std::get<EdgeIndex>(_edges)); // TODO Pass by reference?
				}
				else
				{
					return std::tuple<>();
				}
			}

			template<std::size_t NodeIndex, std::size_t... EdgeIndices>
			auto _getInEdgesForNode(std::index_sequence<EdgeIndices...>)
			{
				auto inEdges = std::tuple_cat(_conditionalInEdges<NodeIndex, EdgeIndices>);
				return inEdges;
			}

			template<std::size_t NodeIndex, std::size_t... EdgeIndices>
			auto _getOutEdgesForNode(std::index_sequence<EdgeIndices...>)
			{
				auto outEdges = std::tuple_cat(_conditionalOutEdges<NodeIndex, EdgeIndices>);
				return outEdges;
			}

			template<std::size_t NodeIndex>
			void _startNodeAtIndex()
			{
				constexpr std::size_t numEdges = std::tuple_size_v<EdgesTuple>;
				auto inEdges = _getInEdgesForNode<NodeIndex>(std::make_index_sequence<numEdges>{});
				auto outEdges = _getOutEdgesForNode<NodeIndex>(std::make_index_sequence<numEdges>{});
				std::get<NodeIndex>(_nodes).start(inEdges, outEdges);
			}

			template<typename Tuple, typename ContextTuple, std::size_t... Indices>
			static Tuple _constructTuple(ContextTuple& contexts, std::index_sequence<Indices...>)
			{
				Tuple tuple = {std::tuple_element_t<Indices, Tuple>(std::get<Indices>(contexts))...};
				return tuple;
			}
			NodesTuple _nodes;
			EdgesTuple _edges;
		};




 */
