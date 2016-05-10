package com.example.android.sacconnecte;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class Empreinte extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_empreinte);
    }

    public void buttonNewEmpreinte(View view){

    }

    public void buttonRetour2(View view){
        finish();
    }
}
