package org.example;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;

public class Connect {
    private static final String URL = "jdbc:postgresql://localhost:5432/univ";
    private static final String USER = "postgres";
    private static final String PASSWORD = "postgres";
    private Connection connection;

    public  Connection connect() throws SQLException {
        if(connection == null || connection.isClosed()){
            connection = DriverManager.getConnection(URL, USER, PASSWORD);
        }
        return connection;
    }
}
