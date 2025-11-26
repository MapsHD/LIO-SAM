#ifndef FLANN_SERIALIZATION_FIX_H
#define FLANN_SERIALIZATION_FIX_H

#include <unordered_map>

// We need to ensure the base Serializer template is visible.
// Depending on the FLANN version and PCL setup, it might be in different headers.
// Including <flann/util/serialization.h> caused issues because it wasn't found in some environments.
// However, since we are specializing a template, we must ensure the primary template is declared.

namespace flann
{
    template<typename T> struct Serializer;
}

namespace flann
{
    // Fix for std::unordered_map serialization (missing in older FLANN versions)
    template<typename K, typename V>
    struct Serializer<std::unordered_map<K, V>>
    {
        template<typename InputArchive>
        static void load(InputArchive& ar, std::unordered_map<K, V>& map)
        {
            size_t size;
            ar & size;
            map.clear();
            for (size_t i = 0; i < size; ++i) {
                K key; V value;
                ar & key; ar & value;
                map[key] = value;
            }
        }
        template<typename OutputArchive>
        static void save(OutputArchive& ar, const std::unordered_map<K, V>& map)
        {
            size_t size = map.size();
            ar & size;
            for (const auto& kv : map) { ar & kv.first; ar & kv.second; }
        }
    };
}

#endif // FLANN_SERIALIZATION_FIX_H

