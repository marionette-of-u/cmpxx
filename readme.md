# 概要
コンパイルにはGNU MP  
（Unix Like System : http://gmplib.org , Windows : http://www.mpir.org ）  
が必要となります。  

## 動作確認
* MSVC 10.0  
* Clang 3.2

## cmpxx::integer
GNU MPのmpz_classと同じ働きをします。  
右辺値参照が利用可能となっています。

## cmpxx::rational
GNU MPのmpq_classと同じ働きをします。  
右辺値参照が利用可能となっています。

## cmpxx::floating
GNU MPのmpf_classと同じ働きをします。  
右辺値参照が利用可能となっています。
