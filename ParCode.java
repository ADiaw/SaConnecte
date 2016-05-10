package com.example.android.sacconnecte;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class ParCode extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_par_code);
    }

    public void buttonOpen(View view){

    }

    public void buttonChangeCode(View view){
        startActivity(new Intent(ParCode.this,ModifierA.class));
    }

    public void buttonRetour(View view){
        startActivity(new Intent(ParCode.this,MainActivity.class));
    }
}
