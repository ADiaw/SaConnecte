package com.example.android.sacconnecte;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void buttonCode(View view){
        startActivity(new Intent(MainActivity.this,ParCode.class));
    }

    public void buttonSave(View view){
        startActivity(new Intent(MainActivity.this,Empreinte.class));
    }

    public void buttonAdresse(View view){
        startActivity(new Intent(MainActivity.this,Adresses.class));
    }
}
