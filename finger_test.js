const finger = require('./finger');

const template = "SjJTUzIxAAADcXIECAUHCc7QAAAvcHYBAAAAg5wgrHHZAOIMiwAdAON/1gD0AGQHWAD4cWELugBuAKML93HyAFoHBQFVAGh+1ABdAO4L3gFkcQEMYwApASgB1XEKAPAPsgAcAON9hgCTAOECPQC0cV4IegCLAIsCeXHrAFoKlgDAAeJwaQDoAOMMqgAJcGcFYgAfAasFTXE4AfQDCQHJAPd+lQCWAF4JKACUce8M+wCfADYKAnDQAFUNsQDOAWRweQDzANkK1gHLcUYLbAAWAScF/XFHAG4PFgHpAHZ/rQUiBfKGxIr5eaMBHQ+G/3KEe/I8+6qKBni+CoD2pPv2BQMVqHmKD2cO/oh/gAIDJebWlD+PUIOgDfqKkv27h6d/6QEhc/uCqIc1lEJ/6h9UDzoYSWyA8yCacYKteQV+vYOqDjcJoYm1DRN9Z/Zw+umOUIMxj+p7cAtqiGsUe4EMdNaLrH+1E8b6qo2IhsaRJQSgidbnGBHd+379cAum+gcHcIJB+Tx2qoTT6OdUaH/Fd/KE8/k2c3+Ae4FpeEp38uW7uuU2AnDOFn8EAP/FdF98AA8AbUX/BlJWegGPT+trSjpLBnHAAWJMCAARAmqwVFsDASoJssALcdEQa1zAYdcBKmJ7amptVv4GaBNwKB56wG9mBVhxfgAVKnqFwpz/w7HAwMEPARX1cPwbwFn/wG0QxCszAUbBwcHAWwdmBHApPndvSgTF+0sBdAQABUk9/RQCWEh6dcD/cQRlwI5EwAMAYlCowhdwK1N9wsBwrMBpKsHAwBMBKpl9wbFxeMB7wUwFDgJnaXeFwmpqjwUDyWxrwmUHAHxyYSlnEAEqdIYHkMGOw2Ro/xEB7HyAs4HC/8JlV5AFAliHhsKVAwHPjv6PBABfkmkrzAEH5WjBYsBUBsWSmRHA/0kHAOleZsKwWwQBKZ6XdwUCfasG+SINADG3YRTBOP/ENxDEK7TGxsaXwWQ+iwoDi7hcwExBwsABF7hBLQcBEMKMQj9yAf3TWsAExYfYFzYHAKjbazrE/UsTARzfw8IHx86y/v9Pwf/8AD8AYe4IYsEFABH3YbFCBQDi92KlBAP8+W08BQCaO14ochFLF2vCA9VeHQXBAxBhI2s6AxOpLlz8BBCYjQD8sgUQj1MGwzgDE0VQOv4DEHeSBvw=";

finger.verify(template, (err, result) => {
    console.log(`err: ${err}, result: ${result}`);
    if (result === "verified") {
        console.log("verified");
    } else {
        console.log("not verified");
    }
});

finger.enroll((err, result) => {
    if (err) {
        console.log("Error: " + err)
    } else {
        console.log("enrolled:");
        console.log(result)
    }
});