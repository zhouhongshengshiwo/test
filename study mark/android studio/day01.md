# mainactivity1
```java
package com.example.myjavacpp;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;
public class MainActivity extends AppCompatActivity {
    private static final int REQUEST_CODE = 1; // 请求码
    private EditText editText;

    @Override//表示这个子类是重写从父类（ppCompatActivity）中继承而来的，且可以防止继承错误
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);//创建activity
        setContentView(R.layout.activity_main);//设置activity_main.xml为当前活动的布局

        editText = findViewById(R.id.editText);//获取editText控件
        Button button = findViewById(R.id.button);//获取button控件

        button.setOnClickListener(new View.OnClickListener() {//activity_main.xml中定义的按钮
            @Override
            public void onClick(View v) {//点击监控器
                // 启动子活动
                Intent intent = new Intent(MainActivity.this, MainActivity2.class);//指定启动的活动，即子活动
                // 传递数据
                intent.putExtra("data", editText.getText().toString());//获取editText中输入的内容."data"为自定义的key,用来作为标识符，让子活动接受能够正确地获取传递的数据
                startActivityForResult(intent, REQUEST_CODE);//启动子活动并传递请求码
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {//startActivityForResult 时指定的请求码将会在这里传递回来,用来判定是子活动传来的数据
        super.onActivityResult(requestCode, resultCode, data);//调用父类的 onActivityResult 方法，让父类处理一些必要的逻辑
        if (requestCode == REQUEST_CODE && resultCode == RESULT_OK) {
            // 接收来自子活动的数据
            String result = data.getStringExtra("result");//返回的数据（以键 result 存储）
            Toast.makeText(this, "Received: " + result, Toast.LENGTH_SHORT).show();
        }
    }
}
```
# mainactivity2
```java
// SubActivity.java
package com.example.myjavacpp;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;

import androidx.appcompat.app.AppCompatActivity;

public class MainActivity2 extends AppCompatActivity {
    private EditText editText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main2);

        editText = findViewById(R.id.editText);
        Button button = findViewById(R.id.button);

        // 接收来自主活动的数据
        String data = getIntent().getStringExtra("data");
        editText.setText(data); // 显示在 EditText 中

        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                // 创建意图返回数据
                Intent resultIntent = new Intent();
                resultIntent.putExtra("result", editText.getText().toString());
                setResult(RESULT_OK, resultIntent);
                finish(); // 结束当前活动，返回主活动
            }
        });
    }
}
```

# 设计Spinner，用于选择所在城市名
## 代码
```java
package com.example.Myproject;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
public class MainActivity3 extends AppCompatActivity {
    private Spinner citySpinner;
    private TextView selectedCityText;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main3);
        citySpinner=findViewById(R.id.CitySpinner);
        selectedCityText=findViewById(R.id.selectCityText);
        ArrayAdapter<CharSequence> adapter=ArrayAdapter.createFromResource(
                this,R.array.cities_array,android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        citySpinner.setAdapter(adapter);

        citySpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            //new AdapterView.OnItemSelectedListener() 是一个接口的实现，它用于监听用户在 Spinner 上选择的项目变化
            //AdapterView.OnItemSelectedListener 接口定义了两个方法，这些方法会在用户选择不同项目时被调用。
            @Override
            public void onItemSelected(AdapterView<?> parentView, View selectedItemView, int position, long id) {
                //parentView 代表当前的 Spinner，也就是 AdapterView。
                //selectedItemView 代表当前被选中的项目的视图
                //position 代表当前被选中的项目的索引，例如，如果用户选择了 Spinner 中的第二个项目，position 的值将为 1（因为索引从 0 开始
                //id 代表当前被选中的项目的行 ID

                //通过实现 OnItemSelectedListener 接口，你可以接收用户选择的事件。
                // 这样你就可以在用户选择不同选项时执行相应的逻辑，例如更新界面的其他部分或执行计算
                // 获取选中的城市并显示
                String selectedCity = parentView.getItemAtPosition(position).toString();//getItemAtPosition(position) 将返回与该索引对应的数据项(城市的字符串)
                selectedCityText.setText("选择的城市: " + selectedCity);
            }

            @Override
            public void onNothingSelected(AdapterView<?> parentView) {
                // 当未选择任何项时的逻辑（可留空）
            }
        });
    }
}
```
AdapterView.OnItemSelectedListener 接口定义了两个方法：
其中：
OnItemSelectedListener 接口中的两个核心方法：
onItemSelected(AdapterView<?> parent, View view, int position, long id)：

parent：表示发生选择事件的 AdapterView（即这里的 citySpinner）。
view：表示被选中的视图，通常是在 Spinner 中的某个项目的视图。
position：表示用户所选项目的位置（索引）。
id：表示用户选择的项目的行 ID，通常在数据集中是唯一的。
# 使用BaseAdapter适配器为ListView绑定数据，并且动态定义显示效果。在网站名及网址信息后添加一个按钮控件，当滚动时在标题栏显示其网址信息；当单击每个条目中的按钮时，在标题栏显示单击的条目ID和网址。

