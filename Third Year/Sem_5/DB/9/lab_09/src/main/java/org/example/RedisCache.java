package org.example;

import redis.clients.jedis.Jedis;

public class RedisCache {
    private Jedis jedis;

    public RedisCache() {
        this.jedis = new Jedis("localhost", 6379);
    }


    public void save(String key, String value) {
        jedis.set(key, value);
    }


    public String get(String key) {
        return jedis.get(key);
    }


    public boolean exists(String key) {
        return jedis.exists(key);
    }
}
