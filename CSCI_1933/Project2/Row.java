public class Row {

  private Object[] data; //array of objects containing data
  private InterpretedQuery iq; //query
  private String[] types; //column types

  //constructs Row object given a query and an array of types
  public Row(InterpretedQuery iq, String[] types) {
    this.iq = iq;
    this.types = types;
    data = new Object[iq.getInsertValues().length];
    addInfo(null);
  }

  //constructs Row object given an array of strings and an array of types
  public Row(String[] stringInfo, String[] types) {
    this.types = types;
    this.data = new Object[stringInfo.length];
    addInfo(stringInfo);
  }

  //adds all information to a row
  private void addInfo(String[] stringInfo) {
    String[] info = null;
    if (stringInfo == null) {
      info = iq.getInsertValues(); //infor from query
    }
    else {
      info = stringInfo; //info from string array
    }

    //converts to appropriate type and stores in data 
    for (int i = 0; i < data.length; i++) {
      switch(types[i]) {
        case "String": String convertedString = info[i];
          data[i] = convertedString;
          break;
        case "int": int convertedInt = Integer.valueOf(info[i]);
          data[i] = convertedInt;
          break;
        case "double": double convertedDouble = Double.valueOf(info[i]);
          data[i] = convertedDouble;
          break;
        case "boolean": boolean convertedBoolean = Boolean.valueOf(info[i]);
          data[i] = convertedBoolean;
          break;
      } //switch
    } //for
  } //addInfo

  public Object[] getData(){
    return data;
  }

}
