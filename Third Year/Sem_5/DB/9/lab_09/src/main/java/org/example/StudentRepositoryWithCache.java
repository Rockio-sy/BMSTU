package org.example;

import com.fasterxml.jackson.databind.ObjectMapper;

import java.util.HashMap;
import java.util.List;


public class StudentRepositoryWithCache {
    private final RedisCache redisCache;
    private final StudentRepository studentRepository;
    private final ObjectMapper objectMapper;
    Connect connect;

    public StudentRepositoryWithCache(ObjectMapper objectMapper, RedisCache redisCache, StudentRepository studentRepository) {
        this.objectMapper = objectMapper;
        this.redisCache = redisCache;
        this.studentRepository = studentRepository;
    }

    public List<Student> getTopTenByAge() {
        String cacheKey = "topTenByAge";
        try {
            if (redisCache.exists(cacheKey)) {
                String cacheData = redisCache.get(cacheKey);
                return objectMapper.readValue(cacheData, objectMapper.getTypeFactory().constructCollectionType(List.class, Student.class));
            } else {
                List<Student> topTen = studentRepository.getTopByAge();
                redisCache.save(cacheKey, objectMapper.writeValueAsString(topTen));
                return topTen;
            }
        } catch (Exception e) {
            System.out.println(e.getMessage());
            return null;
        }
    }

    public List<HashMap<String, Object>> getTotalStudentsByClassGroup() {
        String cacheKey = "totalStudentsByClassGroup";

        try {
            if (redisCache.exists(cacheKey)) {
                // Fetch from cache
                String cachedData = redisCache.get(cacheKey);

                // Deserialize JSON into a List of HashMaps
                return objectMapper.readValue(cachedData,
                        objectMapper.getTypeFactory().constructCollectionType(List.class, HashMap.class));
            } else {
                // Fetch from database
                List<HashMap<String, Object>> classGroupStats = studentRepository.getTotalByClassGroup();

                // Save to cache
                redisCache.save(cacheKey, objectMapper.writeValueAsString(classGroupStats));
                return classGroupStats;
            }
        } catch (Exception e) {
            System.out.println("Error: " + e.getMessage());
            return null;
        }
    }
}