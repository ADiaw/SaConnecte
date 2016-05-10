package com.example.android.sacconnecte;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class Adresses extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_adresses);
    }

    public void buttonSaveAdresse(View view){

    }

    public void buttonRetour3(View view){
        finish();
    }
}
