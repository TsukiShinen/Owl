#pragma once

namespace Owl
{
	class FileDialogs
	{
	public:
		static std::optional<std::filesystem::path> SaveFile(const char* pFilter);
		static std::optional<std::filesystem::path> OpenFile(const char* pFilter);
	};
}
