package org.example;

import com.fasterxml.jackson.databind.ObjectMapper;

import javax.swing.*;
import java.sql.Connection;
import java.sql.SQLException;

public class App
{
    static  Connect connect = new Connect();
    static StudentRepository repo = new StudentRepository(connect);
    static StudentRepositoryWithCache repoWithCache = new StudentRepositoryWithCache(new ObjectMapper(), new RedisCache(), repo);
    static QueryPerformanceTest performanceTest = new QueryPerformanceTest(repo, repoWithCache);

    public static void main( String[] args )
    {
        performanceTest.startPerformanceTest();
    }
}
