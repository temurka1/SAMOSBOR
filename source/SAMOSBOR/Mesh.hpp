#pragma once
#include <vector>

namespace SAMOSBOR::core::occ
{
	/// <summary>
	/// Mesh data
	/// </summary>
	struct Mesh
	{
		/// <summary>
		/// Vertices
		/// </summary>
		std::vector<float> vertices;
		
		/// <summary>
		/// Normals
		/// </summary>
		std::vector<float> normals;
		
		/// <summary>
		/// Indicies
		/// </summary>
		std::vector<uint32_t> indices;
	};
}