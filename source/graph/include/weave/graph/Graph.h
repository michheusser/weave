// Copyright (c) 2025, Michel Heusser
// All rights reserved
// https://github.com/michheusser

#ifndef GRAPH_H_2025_10_05_14_42_23
#define GRAPH_H_2025_10_05_14_42_23

#include <weave/graph/Node.h>
#include <weave/graph/Edge.h>
#include <weave/user/EdgeTraits.h>

namespace weave
{
	namespace graph
	{
		// TODO MAKE SURE THAT ALL BUFFERS ARE REALLY PASSED BY REFERENCE! (It will be anyways clear if the pipeline is broke)
		template<typename GraphDescriptorType>
		struct GraphDescriptionToTuples;
		template<typename... NodeDescriptorTypes, typename... EdgeDescriptorTypes>
		struct GraphDescriptionToTuples<GraphDescriptor<NodeDescriptorList<NodeDescriptorTypes...>, EdgeDescriptorList<EdgeDescriptorTypes...> > >
		{
			using NodeTuple = std::tuple<Node<NodeDescriptorTypes>...>;
			using NodeContextTuple = std::tuple<typename user::NodeTraits<typename NodeDescriptorTypes::Tag>::ContextType...>;
			using EdgeTuple = std::tuple<Edge<EdgeDescriptorTypes>...>;
			using EdgeContextTuple = std::tuple<typename user::EdgeTraits<typename EdgeDescriptorTypes::Tag>::ContextType...>;
		};

		template<typename NodeType>
		struct ExtractDescriptorParamsFromNode;
		template<typename NodeTag>
		struct ExtractDescriptorParamsFromNode<Node<NodeDescriptor<NodeTag>>>
		{
			using Tag = NodeTag;
		};

		template<typename EdgeType>
		struct ExtractDescriptorParamsFromEdge;
		template<typename EdgeTag, typename FromNodeTag, typename ToNodeTag, size_t numSlots>
		struct ExtractDescriptorParamsFromEdge<Edge<EdgeDescriptor<EdgeTag, FromNodeTag, ToNodeTag, numSlots> > >
		{
			using Tag = EdgeTag;
			using FromNode = FromNodeTag;
			using ToNode = ToNodeTag;
			static constexpr size_t slots = numSlots;
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
				_edges(_constructTuple<EdgeTuple>(edgeContexts, std::make_index_sequence<std::tuple_size_v<EdgeContextTuple> >())),
				_nodes(_constructTuple<NodeTuple>(nodeContexts, std::make_index_sequence<std::tuple_size_v<NodeContextTuple> >()))
			{}

			void start()
			{
				try
				{
					_startEdges(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >());
					_startNodes(std::make_index_sequence<std::tuple_size_v<NodeTuple> >());
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
			}

		private:
			// Deduction can only happen from function arguments (end of the list), the previous template arguments have to be explicit (TupleType)
			template<typename TupleType, typename ContextTupleType, std::size_t... Indices>
			TupleType _constructTuple(ContextTupleType& contexts, std::index_sequence<Indices...> sequence)
			{
				// We pass a dummy object to deduce the indices (not the nicest, but seems to be used a lot). Passing it as template parameter requires too many helper constructs.
				/**
				 * The Edges being created are not copyable/not moveable, so we cannot use a syntax that creates temporaries like this one:
				 * TupleType tuple = {std::tuple_element_t<Indices, TupleType>(std::get<Indices>(contexts))...};
				 */
				return TupleType{std::get<Indices>(contexts)...}; // Copy-elision (C++17) allows us to by-pass the deleted copy/move contructors.
			}

			template<std::size_t... EdgeIndices>
			void _startEdges(std::index_sequence<EdgeIndices...> sequence)
			{
				try
				{
					(std::get<EdgeIndices>(_edges).start(), ...);
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
			}

			template<std::size_t... NodeIndices>
			void _startNodes(std::index_sequence<NodeIndices...> sequence)
			{
				try
				{
					(_startNode<NodeIndices>(), ...);
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
			}

			template<std::size_t NodeIndex>
			void _startNode()
			{
				try
				{
					auto inEdges = _getInEdges<NodeIndex>(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >()); // Dummy to infer edge indeces
					auto outEdges = _getOutEdges<NodeIndex>(std::make_index_sequence<std::tuple_size_v<EdgeTuple> >()); // Idem.
					std::get<NodeIndex>(_nodes).start(inEdges, outEdges);
				}
				catch (error::Exception& exception)
				{
					LOG_ERROR(exception.what());
					throw error::Exception("Failed");
				}
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
			EdgeTuple _edges;
			NodeTuple _nodes;
		};
	}
}

#endif
