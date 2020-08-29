#include "AssetImporter.hpp"

std::unique_ptr<AssetImporter> AssetImporter::ManagerInstance;

std::once_flag AssetImporter::m_onceFlag;
