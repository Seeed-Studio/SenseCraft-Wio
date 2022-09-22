function Decoder(bytes, port) {
 
    var decoded = {};
 
    function transformersint(bytes){
        value = bytes[0] * 256 + bytes[1];
        if (value == 32768){
          return;
        }
        if (value > 32768) {
        value = 32768 - value;
        }
        value = value;
        return value;
    }
    
    function transformersfloat(bytes){
        value = bytes[0] * 256 + bytes[1];
        if (value == 32768){
          return;
        }
        if (value > 32768) {
        value = 32768 - value;
        }
        value = value;
        return value/100;
    }
    
    function transformersAI(bytes){
        value = bytes[0];
        if (value == 255 || value >100){
          return;
        }
        return value;
    }

    if (port == 3) {
      if(bytes[0]==0x40) {
        decoded.light = transformersint(bytes.slice(1, 3));
        decoded.mic = transformersint(bytes.slice(3, 5));
        decoded.imux = transformersfloat(bytes.slice(5, 7));
        decoded.imuy = transformersfloat(bytes.slice(7, 9));
        decoded.imuz = transformersfloat(bytes.slice(9, 11));
      }
      if(bytes[0]==0x42) {
        decoded.temp = transformersfloat(bytes.slice(1, 3));
        decoded.humi = transformersfloat(bytes.slice(3, 5));
        decoded.voc = transformersint(bytes.slice(5, 7));
        decoded.co2 = transformersint(bytes.slice(7, 9));
        decoded.soil = transformersint(bytes.slice(9, 11));
      }
      if(bytes[0]==0x44) {
        decoded.confidence_1 = transformersAI(bytes.slice(2));
        decoded.confidence_2 = transformersAI(bytes.slice(4));
        decoded.confidence_3 = transformersAI(bytes.slice(6));
        decoded.confidence_4 = transformersAI(bytes.slice(8));
        decoded.confidence_5 = transformersAI(bytes.slice(10));
      }
      
    }
 
    return decoded;
}