package com.example.android.sacconnecte;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

public class ModifierA extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_modifier);
    }

    public void buttonRetour4(View view){
        startActivity(new Intent(ModifierA.this, MainActivity.class));
    }

    public void buttonDP(View view){
        startActivity(new Intent(ModifierA.this,ParCode.class));
    }
}
