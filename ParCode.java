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
        //à tester c'est ce qui a fait planter mon ordi, le but serait d'ouvrir un layout en alertdialog. Là j'en ai pris un au pif mais le mieux serait de réussir à faire pareil avec le drawable success ou error
        public void buttonOpen(View view){
        AlertDialog.Builder a_builder = new AlertDialog.Builder(ParCode.this);
        a_builder.setCancelable(false).setNegativeButton("Soit", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                dialog.cancel();
            }
        });
        AlertDialog dialog = a_builder.create();
        LayoutInflater inflater = getLayoutInflater();
        View dialogLayout = inflater.inflate(R.layout.layouterror,null);
        dialog.setView(dialogLayout);
        dialog.requestWindowFeature(Window.FEATURE_NO_TITLE);

        dialog.show();

    }

    public void buttonChangeCode(View view){
        startActivity(new Intent(ParCode.this,ModifierA.class));
    }

    public void buttonRetour(View view){
        startActivity(new Intent(ParCode.this,MainActivity.class));
    }
}
