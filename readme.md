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

## cmpxx::mp_float
製作中です。  
cmpxx::integerを利用した、基数が2^std::numeric_limits&lt;mp_limb_t&gt;::digitsの  
浮動小数点数型として機能します。
