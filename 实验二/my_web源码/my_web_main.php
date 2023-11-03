<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>李潇逸的小小网页</title>
</head>
<body>
    <h1>欢迎来自 <?php 
                echo $_POST["college"];
                echo " 的 ";
                echo $_POST["name"]; 
                if($_POST["gender"]=="male")
                {
                    echo "先生";
                }
                else
                {
                    echo "女士";
                }?> </h1>
    <p><b>这是一个简单的WEB测试页面</b></p>
    <p><strong>开始学习啦~</strong></p>
    <br>
    <video width="320" height="240" autoplay><source src="video\俄军.mp4" type="video/mp4"></video>
    <br>
    <h1>have fun!</h1>
    <p><i>玩的开心！</i></p>
    <img decoding="async" src="image/罗小黑2.gif" width="400" height="200" />
    <hr>
    <p>本页面学习自<a href="https://www.runoob.com/html">菜鸟教程</a></p>
</body>
</html>