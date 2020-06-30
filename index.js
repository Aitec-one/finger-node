const finger = require('bindings')('finger.node');

const enroll = (callback) => {
    finger.enroll(callback);
};

const verify = (data, callback) => {
    finger.verify(data, callback);
};

module.exports = {enroll, verify};
