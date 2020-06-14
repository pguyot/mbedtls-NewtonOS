DefConst('kPrintObject, func(obj)
begin
    local f := func(obj, f)
    begin
        local s;
        if IsArray(obj) then
        begin
            s := "[";
            foreach v in obj do
                s := s & call f with (v, f) & ",";
            s := s & "]";
            return s;
        end;
        if IsFrame(obj) then
        begin
            s := "{";
            foreach k, v in obj do
                s := s & k & ":" & call f with (v, f) & ",";
            s := s & "}";
            return s;
        end;
        if obj = NIL then return "NIL";
        if obj = TRUE then return "TRUE";
        if IsString(obj) then return "\"" & obj & "\"";
        if IsSymbol(obj) then return "'" & obj;
        return SPrintObject(obj);
    end;
    Print(call f with (obj, f));
end);

DefConst('kTestCAPEMStr,
    "-----BEGIN CERTIFICATE-----" & unicodeCR & unicodeLF &
    "MIIDQTCCAimgAwIBAgIBAzANBgkqhkiG9w0BAQsFADA7MQswCQYDVQQGEwJOTDER" & unicodeCR & unicodeLF &
    "MA8GA1UECgwIUG9sYXJTU0wxGTAXBgNVBAMMEFBvbGFyU1NMIFRlc3QgQ0EwHhcN" & unicodeCR & unicodeLF &
    "MTkwMjEwMTQ0NDAwWhcNMjkwMjEwMTQ0NDAwWjA7MQswCQYDVQQGEwJOTDERMA8G" & unicodeCR & unicodeLF &
    "A1UECgwIUG9sYXJTU0wxGTAXBgNVBAMMEFBvbGFyU1NMIFRlc3QgQ0EwggEiMA0G" & unicodeCR & unicodeLF &
    "CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDA3zf8F7vglp0/ht6WMn1EpRagzSHx" & unicodeCR & unicodeLF &
    "mdTs6st8GFgIlKXsm8WL3xoemTiZhx57wI053zhdcHgH057Zk+i5clHFzqMwUqny" & unicodeCR & unicodeLF &
    "50BwFMtEonILwuVA+T7lpg6z+exKY8C4KQB0nFc7qKUEkHHxvYPZP9al4jwqj+8n" & unicodeCR & unicodeLF &
    "YMPGn8u67GB9t+aEMr5P+1gmIgNb1LTV+/Xjli5wwOQuvfwu7uJBVcA0Ln0kcmnL" & unicodeCR & unicodeLF &
    "R7EUQIN9Z/SG9jGr8XmksrUuEvmEF/Bibyc+E1ixVA0hmnM3oTDPb5Lc9un8rNsu" & unicodeCR & unicodeLF &
    "KNF+AksjoBXyOGVkCeoMbo4bF6BxyLObyavpw/LPh5aPgAIynplYb6LVAgMBAAGj" & unicodeCR & unicodeLF &
    "UDBOMAwGA1UdEwQFMAMBAf8wHQYDVR0OBBYEFLRa5KWz3tJS9rnVppUP6z68x/3/" & unicodeCR & unicodeLF &
    "MB8GA1UdIwQYMBaAFLRa5KWz3tJS9rnVppUP6z68x/3/MA0GCSqGSIb3DQEBCwUA" & unicodeCR & unicodeLF &
    "A4IBAQA4qFSCth2q22uJIdE4KGHJsJjVEfw2/xn+MkTvCMfxVrvmRvqCtjE4tKDl" & unicodeCR & unicodeLF &
    "oK4MxFOek07oDZwvtAT9ijn1hHftTNS7RH9zd/fxNpfcHnMZXVC4w4DNA1fSANtW" & unicodeCR & unicodeLF &
    "5sY1JB5Je9jScrsLSS+mAjyv0Ow3Hb2Bix8wu7xNNrV5fIf7Ubm+wt6SqEBxu3Kb" & unicodeCR & unicodeLF &
    "+EfObAT4huf3czznhH3C17ed6NSbXwoXfby7stWUDeRJv08RaFOykf/Aae7bY5PL" & unicodeCR & unicodeLF &
    "yTVrkAnikMntJ9YI+hNNYt3inqq11A5cN0+rVTst8UKCxzQ4GpvroSwPKTFkbMw4" & unicodeCR & unicodeLF &
    "/anT1dVxr/BtwJfiESoK3/4CeXR1" & unicodeCR & unicodeLF &
    "-----END CERTIFICATE-----" & unicodeCR & unicodeLF &
    "-----BEGIN CERTIFICATE-----" & unicodeCR & unicodeLF &
    "MIIDQTCCAimgAwIBAgIBAzANBgkqhkiG9w0BAQUFADA7MQswCQYDVQQGEwJOTDER" & unicodeCR & unicodeLF &
    "MA8GA1UECgwIUG9sYXJTU0wxGTAXBgNVBAMMEFBvbGFyU1NMIFRlc3QgQ0EwHhcN" & unicodeCR & unicodeLF &
    "MTkwMjEwMTQ0NDAwWhcNMjkwMjEwMTQ0NDAwWjA7MQswCQYDVQQGEwJOTDERMA8G" & unicodeCR & unicodeLF &
    "A1UECgwIUG9sYXJTU0wxGTAXBgNVBAMMEFBvbGFyU1NMIFRlc3QgQ0EwggEiMA0G" & unicodeCR & unicodeLF &
    "CSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDA3zf8F7vglp0/ht6WMn1EpRagzSHx" & unicodeCR & unicodeLF &
    "mdTs6st8GFgIlKXsm8WL3xoemTiZhx57wI053zhdcHgH057Zk+i5clHFzqMwUqny" & unicodeCR & unicodeLF &
    "50BwFMtEonILwuVA+T7lpg6z+exKY8C4KQB0nFc7qKUEkHHxvYPZP9al4jwqj+8n" & unicodeCR & unicodeLF &
    "YMPGn8u67GB9t+aEMr5P+1gmIgNb1LTV+/Xjli5wwOQuvfwu7uJBVcA0Ln0kcmnL" & unicodeCR & unicodeLF &
    "R7EUQIN9Z/SG9jGr8XmksrUuEvmEF/Bibyc+E1ixVA0hmnM3oTDPb5Lc9un8rNsu" & unicodeCR & unicodeLF &
    "KNF+AksjoBXyOGVkCeoMbo4bF6BxyLObyavpw/LPh5aPgAIynplYb6LVAgMBAAGj" & unicodeCR & unicodeLF &
    "UDBOMAwGA1UdEwQFMAMBAf8wHQYDVR0OBBYEFLRa5KWz3tJS9rnVppUP6z68x/3/" & unicodeCR & unicodeLF &
    "MB8GA1UdIwQYMBaAFLRa5KWz3tJS9rnVppUP6z68x/3/MA0GCSqGSIb3DQEBBQUA" & unicodeCR & unicodeLF &
    "A4IBAQB0ZiNRFdia6kskaPnhrqejIRq8YMEGAf2oIPnyZ78xoyERgc35lHGyMtsL" & unicodeCR & unicodeLF &
    "hWicNjP4d/hS9As4j5KA2gdNGi5ETA1X7SowWOGsryivSpMSHVy1+HdfWlsYQOzm" & unicodeCR & unicodeLF &
    "8o+faQNUm8XzPVmttfAVspxeHSxJZ36Oo+QWZ5wZlCIEyjEdLUId+Tm4Bz3B5jRD" & unicodeCR & unicodeLF &
    "zZa/SaqDokq66N2zpbgKKAl3GU2O++fBqP2dSkdQykmTxhLLWRN8FJqhYATyQntZ" & unicodeCR & unicodeLF &
    "0QSi3W9HfSZPnFTcPIXeoiPd2pLlxt1hZu8dws2LTXE63uP6MM4LHvWxiuJaWkP/" & unicodeCR & unicodeLF &
    "mtxyUALj2pQxRitopORFQdn7AOY5" & unicodeCR & unicodeLF &
    "-----END CERTIFICATE-----" & unicodeCR & unicodeLF &
    "-----BEGIN CERTIFICATE-----" & unicodeCR & unicodeLF &
    "MIICBDCCAYigAwIBAgIJAMFD4n5iQ8zoMAwGCCqGSM49BAMCBQAwPjELMAkGA1UE" & unicodeCR & unicodeLF &
    "BhMCTkwxETAPBgNVBAoMCFBvbGFyU1NMMRwwGgYDVQQDDBNQb2xhcnNzbCBUZXN0" & unicodeCR & unicodeLF &
    "IEVDIENBMB4XDTE5MDIxMDE0NDQwMFoXDTI5MDIxMDE0NDQwMFowPjELMAkGA1UE" & unicodeCR & unicodeLF &
    "BhMCTkwxETAPBgNVBAoMCFBvbGFyU1NMMRwwGgYDVQQDDBNQb2xhcnNzbCBUZXN0" & unicodeCR & unicodeLF &
    "IEVDIENBMHYwEAYHKoZIzj0CAQYFK4EEACIDYgAEw9orNEE3WC+HVv78ibopQ0tO" & unicodeCR & unicodeLF &
    "4G7DDldTMzlY1FK0kZU5CyPfXxckYkj8GpUpziwth8KIUoCv1mqrId240xxuWLjK" & unicodeCR & unicodeLF &
    "6LJpjvNBrSnDtF91p0dv1RkpVWmaUzsgtGYWYDMeo1AwTjAMBgNVHRMEBTADAQH/" & unicodeCR & unicodeLF &
    "MB0GA1UdDgQWBBSdbSAkSQE/K8t4tRm8fiTJ2/s2fDAfBgNVHSMEGDAWgBSdbSAk" & unicodeCR & unicodeLF &
    "SQE/K8t4tRm8fiTJ2/s2fDAMBggqhkjOPQQDAgUAA2gAMGUCMFHKrjAPpHB0BN1a" & unicodeCR & unicodeLF &
    "LH8TwcJ3vh0AxeKZj30mRdOKBmg/jLS3rU3g8VQBHpn8sOTTBwIxANxPO5AerimZ" & unicodeCR & unicodeLF &
    "hCjMe0d4CTHf1gFZMF70+IqEP+o5VHsIp2Cqvflb0VGWFC5l9a4cQg==" & unicodeCR & unicodeLF &
    "-----END CERTIFICATE-----" & unicodeCR & unicodeLF);

DefConst('kTestCADER_SHA256, [
  0x30, 0x82, 0x03, 0x41, 0x30, 0x82, 0x02, 0x29, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x01, 0x03, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00, 0x30, 0x3b, 0x31, 0x0b, 0x30,
  0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11,
  0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c,
  0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55,
  0x04, 0x03, 0x0c, 0x10, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c,
  0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d,
  0x31, 0x39, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34, 0x34, 0x30, 0x30,
  0x5a, 0x17, 0x0d, 0x32, 0x39, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34,
  0x34, 0x30, 0x30, 0x5a, 0x30, 0x3b, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
  0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06,
  0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53,
  0x53, 0x4c, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
  0x10, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x20, 0x54, 0x65,
  0x73, 0x74, 0x20, 0x43, 0x41, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06,
  0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00,
  0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01,
  0x01, 0x00, 0xc0, 0xdf, 0x37, 0xfc, 0x17, 0xbb, 0xe0, 0x96, 0x9d, 0x3f,
  0x86, 0xde, 0x96, 0x32, 0x7d, 0x44, 0xa5, 0x16, 0xa0, 0xcd, 0x21, 0xf1,
  0x99, 0xd4, 0xec, 0xea, 0xcb, 0x7c, 0x18, 0x58, 0x08, 0x94, 0xa5, 0xec,
  0x9b, 0xc5, 0x8b, 0xdf, 0x1a, 0x1e, 0x99, 0x38, 0x99, 0x87, 0x1e, 0x7b,
  0xc0, 0x8d, 0x39, 0xdf, 0x38, 0x5d, 0x70, 0x78, 0x07, 0xd3, 0x9e, 0xd9,
  0x93, 0xe8, 0xb9, 0x72, 0x51, 0xc5, 0xce, 0xa3, 0x30, 0x52, 0xa9, 0xf2,
  0xe7, 0x40, 0x70, 0x14, 0xcb, 0x44, 0xa2, 0x72, 0x0b, 0xc2, 0xe5, 0x40,
  0xf9, 0x3e, 0xe5, 0xa6, 0x0e, 0xb3, 0xf9, 0xec, 0x4a, 0x63, 0xc0, 0xb8,
  0x29, 0x00, 0x74, 0x9c, 0x57, 0x3b, 0xa8, 0xa5, 0x04, 0x90, 0x71, 0xf1,
  0xbd, 0x83, 0xd9, 0x3f, 0xd6, 0xa5, 0xe2, 0x3c, 0x2a, 0x8f, 0xef, 0x27,
  0x60, 0xc3, 0xc6, 0x9f, 0xcb, 0xba, 0xec, 0x60, 0x7d, 0xb7, 0xe6, 0x84,
  0x32, 0xbe, 0x4f, 0xfb, 0x58, 0x26, 0x22, 0x03, 0x5b, 0xd4, 0xb4, 0xd5,
  0xfb, 0xf5, 0xe3, 0x96, 0x2e, 0x70, 0xc0, 0xe4, 0x2e, 0xbd, 0xfc, 0x2e,
  0xee, 0xe2, 0x41, 0x55, 0xc0, 0x34, 0x2e, 0x7d, 0x24, 0x72, 0x69, 0xcb,
  0x47, 0xb1, 0x14, 0x40, 0x83, 0x7d, 0x67, 0xf4, 0x86, 0xf6, 0x31, 0xab,
  0xf1, 0x79, 0xa4, 0xb2, 0xb5, 0x2e, 0x12, 0xf9, 0x84, 0x17, 0xf0, 0x62,
  0x6f, 0x27, 0x3e, 0x13, 0x58, 0xb1, 0x54, 0x0d, 0x21, 0x9a, 0x73, 0x37,
  0xa1, 0x30, 0xcf, 0x6f, 0x92, 0xdc, 0xf6, 0xe9, 0xfc, 0xac, 0xdb, 0x2e,
  0x28, 0xd1, 0x7e, 0x02, 0x4b, 0x23, 0xa0, 0x15, 0xf2, 0x38, 0x65, 0x64,
  0x09, 0xea, 0x0c, 0x6e, 0x8e, 0x1b, 0x17, 0xa0, 0x71, 0xc8, 0xb3, 0x9b,
  0xc9, 0xab, 0xe9, 0xc3, 0xf2, 0xcf, 0x87, 0x96, 0x8f, 0x80, 0x02, 0x32,
  0x9e, 0x99, 0x58, 0x6f, 0xa2, 0xd5, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3,
  0x50, 0x30, 0x4e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05,
  0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e,
  0x04, 0x16, 0x04, 0x14, 0xb4, 0x5a, 0xe4, 0xa5, 0xb3, 0xde, 0xd2, 0x52,
  0xf6, 0xb9, 0xd5, 0xa6, 0x95, 0x0f, 0xeb, 0x3e, 0xbc, 0xc7, 0xfd, 0xff,
  0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80,
  0x14, 0xb4, 0x5a, 0xe4, 0xa5, 0xb3, 0xde, 0xd2, 0x52, 0xf6, 0xb9, 0xd5,
  0xa6, 0x95, 0x0f, 0xeb, 0x3e, 0xbc, 0xc7, 0xfd, 0xff, 0x30, 0x0d, 0x06,
  0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x0b, 0x05, 0x00,
  0x03, 0x82, 0x01, 0x01, 0x00, 0x38, 0xa8, 0x54, 0x82, 0xb6, 0x1d, 0xaa,
  0xdb, 0x6b, 0x89, 0x21, 0xd1, 0x38, 0x28, 0x61, 0xc9, 0xb0, 0x98, 0xd5,
  0x11, 0xfc, 0x36, 0xff, 0x19, 0xfe, 0x32, 0x44, 0xef, 0x08, 0xc7, 0xf1,
  0x56, 0xbb, 0xe6, 0x46, 0xfa, 0x82, 0xb6, 0x31, 0x38, 0xb4, 0xa0, 0xe5,
  0xa0, 0xae, 0x0c, 0xc4, 0x53, 0x9e, 0x93, 0x4e, 0xe8, 0x0d, 0x9c, 0x2f,
  0xb4, 0x04, 0xfd, 0x8a, 0x39, 0xf5, 0x84, 0x77, 0xed, 0x4c, 0xd4, 0xbb,
  0x44, 0x7f, 0x73, 0x77, 0xf7, 0xf1, 0x36, 0x97, 0xdc, 0x1e, 0x73, 0x19,
  0x5d, 0x50, 0xb8, 0xc3, 0x80, 0xcd, 0x03, 0x57, 0xd2, 0x00, 0xdb, 0x56,
  0xe6, 0xc6, 0x35, 0x24, 0x1e, 0x49, 0x7b, 0xd8, 0xd2, 0x72, 0xbb, 0x0b,
  0x49, 0x2f, 0xa6, 0x02, 0x3c, 0xaf, 0xd0, 0xec, 0x37, 0x1d, 0xbd, 0x81,
  0x8b, 0x1f, 0x30, 0xbb, 0xbc, 0x4d, 0x36, 0xb5, 0x79, 0x7c, 0x87, 0xfb,
  0x51, 0xb9, 0xbe, 0xc2, 0xde, 0x92, 0xa8, 0x40, 0x71, 0xbb, 0x72, 0x9b,
  0xf8, 0x47, 0xce, 0x6c, 0x04, 0xf8, 0x86, 0xe7, 0xf7, 0x73, 0x3c, 0xe7,
  0x84, 0x7d, 0xc2, 0xd7, 0xb7, 0x9d, 0xe8, 0xd4, 0x9b, 0x5f, 0x0a, 0x17,
  0x7d, 0xbc, 0xbb, 0xb2, 0xd5, 0x94, 0x0d, 0xe4, 0x49, 0xbf, 0x4f, 0x11,
  0x68, 0x53, 0xb2, 0x91, 0xff, 0xc0, 0x69, 0xee, 0xdb, 0x63, 0x93, 0xcb,
  0xc9, 0x35, 0x6b, 0x90, 0x09, 0xe2, 0x90, 0xc9, 0xed, 0x27, 0xd6, 0x08,
  0xfa, 0x13, 0x4d, 0x62, 0xdd, 0xe2, 0x9e, 0xaa, 0xb5, 0xd4, 0x0e, 0x5c,
  0x37, 0x4f, 0xab, 0x55, 0x3b, 0x2d, 0xf1, 0x42, 0x82, 0xc7, 0x34, 0x38,
  0x1a, 0x9b, 0xeb, 0xa1, 0x2c, 0x0f, 0x29, 0x31, 0x64, 0x6c, 0xcc, 0x38,
  0xfd, 0xa9, 0xd3, 0xd5, 0xd5, 0x71, 0xaf, 0xf0, 0x6d, 0xc0, 0x97, 0xe2,
  0x11, 0x2a, 0x0a, 0xdf, 0xfe, 0x02, 0x79, 0x74, 0x75
]);
DefConst('kTestCADER_SHA1, [
  0x30, 0x82, 0x03, 0x41, 0x30, 0x82, 0x02, 0x29, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x01, 0x03, 0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
  0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30, 0x3b, 0x31, 0x0b, 0x30,
  0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11,
  0x30, 0x0f, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c,
  0x61, 0x72, 0x53, 0x53, 0x4c, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55,
  0x04, 0x03, 0x0c, 0x10, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c,
  0x20, 0x54, 0x65, 0x73, 0x74, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d,
  0x31, 0x39, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34, 0x34, 0x30, 0x30,
  0x5a, 0x17, 0x0d, 0x32, 0x39, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34,
  0x34, 0x30, 0x30, 0x5a, 0x30, 0x3b, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03,
  0x55, 0x04, 0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06,
  0x03, 0x55, 0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53,
  0x53, 0x4c, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
  0x10, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c, 0x20, 0x54, 0x65,
  0x73, 0x74, 0x20, 0x43, 0x41, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d, 0x06,
  0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01, 0x05, 0x00,
  0x03, 0x82, 0x01, 0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82, 0x01,
  0x01, 0x00, 0xc0, 0xdf, 0x37, 0xfc, 0x17, 0xbb, 0xe0, 0x96, 0x9d, 0x3f,
  0x86, 0xde, 0x96, 0x32, 0x7d, 0x44, 0xa5, 0x16, 0xa0, 0xcd, 0x21, 0xf1,
  0x99, 0xd4, 0xec, 0xea, 0xcb, 0x7c, 0x18, 0x58, 0x08, 0x94, 0xa5, 0xec,
  0x9b, 0xc5, 0x8b, 0xdf, 0x1a, 0x1e, 0x99, 0x38, 0x99, 0x87, 0x1e, 0x7b,
  0xc0, 0x8d, 0x39, 0xdf, 0x38, 0x5d, 0x70, 0x78, 0x07, 0xd3, 0x9e, 0xd9,
  0x93, 0xe8, 0xb9, 0x72, 0x51, 0xc5, 0xce, 0xa3, 0x30, 0x52, 0xa9, 0xf2,
  0xe7, 0x40, 0x70, 0x14, 0xcb, 0x44, 0xa2, 0x72, 0x0b, 0xc2, 0xe5, 0x40,
  0xf9, 0x3e, 0xe5, 0xa6, 0x0e, 0xb3, 0xf9, 0xec, 0x4a, 0x63, 0xc0, 0xb8,
  0x29, 0x00, 0x74, 0x9c, 0x57, 0x3b, 0xa8, 0xa5, 0x04, 0x90, 0x71, 0xf1,
  0xbd, 0x83, 0xd9, 0x3f, 0xd6, 0xa5, 0xe2, 0x3c, 0x2a, 0x8f, 0xef, 0x27,
  0x60, 0xc3, 0xc6, 0x9f, 0xcb, 0xba, 0xec, 0x60, 0x7d, 0xb7, 0xe6, 0x84,
  0x32, 0xbe, 0x4f, 0xfb, 0x58, 0x26, 0x22, 0x03, 0x5b, 0xd4, 0xb4, 0xd5,
  0xfb, 0xf5, 0xe3, 0x96, 0x2e, 0x70, 0xc0, 0xe4, 0x2e, 0xbd, 0xfc, 0x2e,
  0xee, 0xe2, 0x41, 0x55, 0xc0, 0x34, 0x2e, 0x7d, 0x24, 0x72, 0x69, 0xcb,
  0x47, 0xb1, 0x14, 0x40, 0x83, 0x7d, 0x67, 0xf4, 0x86, 0xf6, 0x31, 0xab,
  0xf1, 0x79, 0xa4, 0xb2, 0xb5, 0x2e, 0x12, 0xf9, 0x84, 0x17, 0xf0, 0x62,
  0x6f, 0x27, 0x3e, 0x13, 0x58, 0xb1, 0x54, 0x0d, 0x21, 0x9a, 0x73, 0x37,
  0xa1, 0x30, 0xcf, 0x6f, 0x92, 0xdc, 0xf6, 0xe9, 0xfc, 0xac, 0xdb, 0x2e,
  0x28, 0xd1, 0x7e, 0x02, 0x4b, 0x23, 0xa0, 0x15, 0xf2, 0x38, 0x65, 0x64,
  0x09, 0xea, 0x0c, 0x6e, 0x8e, 0x1b, 0x17, 0xa0, 0x71, 0xc8, 0xb3, 0x9b,
  0xc9, 0xab, 0xe9, 0xc3, 0xf2, 0xcf, 0x87, 0x96, 0x8f, 0x80, 0x02, 0x32,
  0x9e, 0x99, 0x58, 0x6f, 0xa2, 0xd5, 0x02, 0x03, 0x01, 0x00, 0x01, 0xa3,
  0x50, 0x30, 0x4e, 0x30, 0x0c, 0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05,
  0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e,
  0x04, 0x16, 0x04, 0x14, 0xb4, 0x5a, 0xe4, 0xa5, 0xb3, 0xde, 0xd2, 0x52,
  0xf6, 0xb9, 0xd5, 0xa6, 0x95, 0x0f, 0xeb, 0x3e, 0xbc, 0xc7, 0xfd, 0xff,
  0x30, 0x1f, 0x06, 0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80,
  0x14, 0xb4, 0x5a, 0xe4, 0xa5, 0xb3, 0xde, 0xd2, 0x52, 0xf6, 0xb9, 0xd5,
  0xa6, 0x95, 0x0f, 0xeb, 0x3e, 0xbc, 0xc7, 0xfd, 0xff, 0x30, 0x0d, 0x06,
  0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00,
  0x03, 0x82, 0x01, 0x01, 0x00, 0x74, 0x66, 0x23, 0x51, 0x15, 0xd8, 0x9a,
  0xea, 0x4b, 0x24, 0x68, 0xf9, 0xe1, 0xae, 0xa7, 0xa3, 0x21, 0x1a, 0xbc,
  0x60, 0xc1, 0x06, 0x01, 0xfd, 0xa8, 0x20, 0xf9, 0xf2, 0x67, 0xbf, 0x31,
  0xa3, 0x21, 0x11, 0x81, 0xcd, 0xf9, 0x94, 0x71, 0xb2, 0x32, 0xdb, 0x0b,
  0x85, 0x68, 0x9c, 0x36, 0x33, 0xf8, 0x77, 0xf8, 0x52, 0xf4, 0x0b, 0x38,
  0x8f, 0x92, 0x80, 0xda, 0x07, 0x4d, 0x1a, 0x2e, 0x44, 0x4c, 0x0d, 0x57,
  0xed, 0x2a, 0x30, 0x58, 0xe1, 0xac, 0xaf, 0x28, 0xaf, 0x4a, 0x93, 0x12,
  0x1d, 0x5c, 0xb5, 0xf8, 0x77, 0x5f, 0x5a, 0x5b, 0x18, 0x40, 0xec, 0xe6,
  0xf2, 0x8f, 0x9f, 0x69, 0x03, 0x54, 0x9b, 0xc5, 0xf3, 0x3d, 0x59, 0xad,
  0xb5, 0xf0, 0x15, 0xb2, 0x9c, 0x5e, 0x1d, 0x2c, 0x49, 0x67, 0x7e, 0x8e,
  0xa3, 0xe4, 0x16, 0x67, 0x9c, 0x19, 0x94, 0x22, 0x04, 0xca, 0x31, 0x1d,
  0x2d, 0x42, 0x1d, 0xf9, 0x39, 0xb8, 0x07, 0x3d, 0xc1, 0xe6, 0x34, 0x43,
  0xcd, 0x96, 0xbf, 0x49, 0xaa, 0x83, 0xa2, 0x4a, 0xba, 0xe8, 0xdd, 0xb3,
  0xa5, 0xb8, 0x0a, 0x28, 0x09, 0x77, 0x19, 0x4d, 0x8e, 0xfb, 0xe7, 0xc1,
  0xa8, 0xfd, 0x9d, 0x4a, 0x47, 0x50, 0xca, 0x49, 0x93, 0xc6, 0x12, 0xcb,
  0x59, 0x13, 0x7c, 0x14, 0x9a, 0xa1, 0x60, 0x04, 0xf2, 0x42, 0x7b, 0x59,
  0xd1, 0x04, 0xa2, 0xdd, 0x6f, 0x47, 0x7d, 0x26, 0x4f, 0x9c, 0x54, 0xdc,
  0x3c, 0x85, 0xde, 0xa2, 0x23, 0xdd, 0xda, 0x92, 0xe5, 0xc6, 0xdd, 0x61,
  0x66, 0xef, 0x1d, 0xc2, 0xcd, 0x8b, 0x4d, 0x71, 0x3a, 0xde, 0xe3, 0xfa,
  0x30, 0xce, 0x0b, 0x1e, 0xf5, 0xb1, 0x8a, 0xe2, 0x5a, 0x5a, 0x43, 0xff,
  0x9a, 0xdc, 0x72, 0x50, 0x02, 0xe3, 0xda, 0x94, 0x31, 0x46, 0x2b, 0x68,
  0xa4, 0xe4, 0x45, 0x41, 0xd9, 0xfb, 0x00, 0xe6, 0x39
]);
DefConst('kTestCADER_EC, [
  0x30, 0x82, 0x02, 0x04, 0x30, 0x82, 0x01, 0x88, 0xa0, 0x03, 0x02, 0x01,
  0x02, 0x02, 0x09, 0x00, 0xc1, 0x43, 0xe2, 0x7e, 0x62, 0x43, 0xcc, 0xe8,
  0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x04, 0x03, 0x02,
  0x05, 0x00, 0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
  0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55,
  0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c,
  0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x13, 0x50,
  0x6f, 0x6c, 0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74,
  0x20, 0x45, 0x43, 0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d, 0x31, 0x39,
  0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34, 0x34, 0x30, 0x30, 0x5a, 0x17,
  0x0d, 0x32, 0x39, 0x30, 0x32, 0x31, 0x30, 0x31, 0x34, 0x34, 0x34, 0x30,
  0x30, 0x5a, 0x30, 0x3e, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
  0x06, 0x13, 0x02, 0x4e, 0x4c, 0x31, 0x11, 0x30, 0x0f, 0x06, 0x03, 0x55,
  0x04, 0x0a, 0x0c, 0x08, 0x50, 0x6f, 0x6c, 0x61, 0x72, 0x53, 0x53, 0x4c,
  0x31, 0x1c, 0x30, 0x1a, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c, 0x13, 0x50,
  0x6f, 0x6c, 0x61, 0x72, 0x73, 0x73, 0x6c, 0x20, 0x54, 0x65, 0x73, 0x74,
  0x20, 0x45, 0x43, 0x20, 0x43, 0x41, 0x30, 0x76, 0x30, 0x10, 0x06, 0x07,
  0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x05, 0x2b, 0x81, 0x04,
  0x00, 0x22, 0x03, 0x62, 0x00, 0x04, 0xc3, 0xda, 0x2b, 0x34, 0x41, 0x37,
  0x58, 0x2f, 0x87, 0x56, 0xfe, 0xfc, 0x89, 0xba, 0x29, 0x43, 0x4b, 0x4e,
  0xe0, 0x6e, 0xc3, 0x0e, 0x57, 0x53, 0x33, 0x39, 0x58, 0xd4, 0x52, 0xb4,
  0x91, 0x95, 0x39, 0x0b, 0x23, 0xdf, 0x5f, 0x17, 0x24, 0x62, 0x48, 0xfc,
  0x1a, 0x95, 0x29, 0xce, 0x2c, 0x2d, 0x87, 0xc2, 0x88, 0x52, 0x80, 0xaf,
  0xd6, 0x6a, 0xab, 0x21, 0xdd, 0xb8, 0xd3, 0x1c, 0x6e, 0x58, 0xb8, 0xca,
  0xe8, 0xb2, 0x69, 0x8e, 0xf3, 0x41, 0xad, 0x29, 0xc3, 0xb4, 0x5f, 0x75,
  0xa7, 0x47, 0x6f, 0xd5, 0x19, 0x29, 0x55, 0x69, 0x9a, 0x53, 0x3b, 0x20,
  0xb4, 0x66, 0x16, 0x60, 0x33, 0x1e, 0xa3, 0x50, 0x30, 0x4e, 0x30, 0x0c,
  0x06, 0x03, 0x55, 0x1d, 0x13, 0x04, 0x05, 0x30, 0x03, 0x01, 0x01, 0xff,
  0x30, 0x1d, 0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04, 0x14, 0x9d,
  0x6d, 0x20, 0x24, 0x49, 0x01, 0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc,
  0x7e, 0x24, 0xc9, 0xdb, 0xfb, 0x36, 0x7c, 0x30, 0x1f, 0x06, 0x03, 0x55,
  0x1d, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x9d, 0x6d, 0x20, 0x24,
  0x49, 0x01, 0x3f, 0x2b, 0xcb, 0x78, 0xb5, 0x19, 0xbc, 0x7e, 0x24, 0xc9,
  0xdb, 0xfb, 0x36, 0x7c, 0x30, 0x0c, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce,
  0x3d, 0x04, 0x03, 0x02, 0x05, 0x00, 0x03, 0x68, 0x00, 0x30, 0x65, 0x02,
  0x30, 0x51, 0xca, 0xae, 0x30, 0x0f, 0xa4, 0x70, 0x74, 0x04, 0xdd, 0x5a,
  0x2c, 0x7f, 0x13, 0xc1, 0xc2, 0x77, 0xbe, 0x1d, 0x00, 0xc5, 0xe2, 0x99,
  0x8f, 0x7d, 0x26, 0x45, 0xd3, 0x8a, 0x06, 0x68, 0x3f, 0x8c, 0xb4, 0xb7,
  0xad, 0x4d, 0xe0, 0xf1, 0x54, 0x01, 0x1e, 0x99, 0xfc, 0xb0, 0xe4, 0xd3,
  0x07, 0x02, 0x31, 0x00, 0xdc, 0x4f, 0x3b, 0x90, 0x1e, 0xae, 0x29, 0x99,
  0x84, 0x28, 0xcc, 0x7b, 0x47, 0x78, 0x09, 0x31, 0xdf, 0xd6, 0x01, 0x59,
  0x30, 0x5e, 0xf4, 0xf8, 0x8a, 0x84, 0x3f, 0xea, 0x39, 0x54, 0x7b, 0x08,
  0xa7, 0x60, 0xaa, 0xbd, 0xf9, 0x5b, 0xd1, 0x51, 0x96, 0x14, 0x2e, 0x65,
  0xf5, 0xae, 0x1c, 0x42
]);

DefConst('kPEMParseTest, {
    mbedTLS: UR(kMbedTLSSymbol, 'mbedTLS),
    Run: func()
    begin
        local x509CertBin := mbedTLS:X509CrtNew();
        local count := mbedTLS:X509CrtCount(x509CertBin);
        Print("New count: " & count);
        if count <> 0 then return :TestEndCb(nil);
        local r := mbedTLS:X509CrtParse(x509CertBin, kTestCAPEMStr);
        if r <> 'ok then return :TestEndCb(nil);
        count := mbedTLS:X509CrtCount(x509CertBin);
        Print("Parse count: " & count);
        if count <> 3 then return :TestEndCb(nil);
        for ix := 0 to count - 1 do
        begin
            Print("Certificate #" & count);
            local der := mbedTLS:X509CrtGetDER(x509CertBin, ix);
            Print(" DER: <" & Length(der) & ">");
            local version := mbedTLS:X509CrtGetVersion(x509CertBin, ix);
            Print(" Version: " & version);
            if version <> 3 then return :TestEndCb(nil);
            local serialNumber := mbedTLS:X509CrtGetSerialNumber(x509CertBin, ix);
            Print(" Serial number: " & serialNumber);
            local subjectName := mbedTLS:X509CrtGetSubjectName(x509CertBin, ix);
            Print(" Subject name: " & subjectName);
            local issueDate := mbedTLS:X509CrtGetIssueDate(x509CertBin, ix);
            Print(" Issue date: " & DateNTime(issueDate));
            local expirationDate := mbedTLS:X509CrtGetExpirationDate(x509CertBin, ix);
            Print(" Expiration date: " & DateNTime(expirationDate));
            local signatureAlgorithm := mbedTLS:X509CrtGetSignatureAlgorithm(x509CertBin, ix);
            Print(" Signature algorithm: " & signatureAlgorithm);
        end;
        :TestEndCb(true);
    end
});

DefConst('kDERParseTest, {
    mbedTLS: UR(kMbedTLSSymbol, 'mbedTLS),
    Run: func()
    begin
        foreach cert in [kTestCADER_SHA256, kTestCADER_SHA1, kTestCADER_EC] do
        begin
            Print("Certificate: " & Length(cert) & " bytes");
            local x509CertBin := mbedTLS:X509CrtNew();
            local count := mbedTLS:X509CrtCount(x509CertBin);
            Print("New count: " & count);
            if count <> 0 then return :TestEndCb(nil);
            local r := mbedTLS:X509CrtParse(x509CertBin, cert);
            if r <> 'ok then return :TestEndCb(nil);
            count := mbedTLS:X509CrtCount(x509CertBin);
            Print("Parse count: " & count);
            if count <> 1 then return :TestEndCb(nil);
            local der := mbedTLS:X509CrtGetDER(x509CertBin, 0);
            Print(" DER: <" & Length(der) & ">");
            if Length(der) <> Length(cert) then return :TestEndCb(nil);
            local version := mbedTLS:X509CrtGetVersion(x509CertBin, 0);
            Print(" Version: " & version);
            if version <> 3 then return :TestEndCb(nil);
            local serialNumber := mbedTLS:X509CrtGetSerialNumber(x509CertBin, 0);
            Print(" Serial number: " & serialNumber);
            local subjectName := mbedTLS:X509CrtGetSubjectName(x509CertBin, 0);
            Print(" Subject name: " & subjectName);
            local issueDate := mbedTLS:X509CrtGetIssueDate(x509CertBin, 0);
            Print(" Issue date: " & DateNTime(issueDate));
            local expirationDate := mbedTLS:X509CrtGetExpirationDate(x509CertBin, 0);
            Print(" Expiration date: " & DateNTime(expirationDate));
            local signatureAlgorithm := mbedTLS:X509CrtGetSignatureAlgorithm(x509CertBin, 0);
            Print(" Signature algorithm: " & signatureAlgorithm);
        end;
        :TestEndCb(true);
    end
});

DefConst('kParseRootCertificatesTest, {
    mbedTLS: UR(kMbedTLSSymbol, 'mbedTLS),
    Run: func()
    begin
        local packageStore := GetPackageStore("CACertificates:Kallisys");
        local packageSoup := packageStore:GetSoup("CACertificates:Kallisys");
        local packageCursor := packageSoup:query(nil);
        loop begin
            local entry := packageCursor:entry();
            if entry = nil then break;
            local x509CertBin := mbedTLS:X509CrtNew();
            local count := mbedTLS:X509CrtCount(x509CertBin);
            if count <> 0 then return :TestEndCb(nil);
            local r;
            Print("Entry " & entry.tag);
            if entry.pem then
                r := mbedTLS:X509CrtParse(x509CertBin, entry.pem)
            else
                r := mbedTLS:X509CrtParse(x509CertBin, entry.der);
            if r <> 'ok then begin
                Print("Could not parse it !");
                return :TestEndCb(nil);
            end;
            count := mbedTLS:X509CrtCount(x509CertBin);
            Print(" Certificates count: " & count);
            for ix := 0 to count - 1 do
            begin
                Print(" Certificate #" & count);
                local der := mbedTLS:X509CrtGetDER(x509CertBin, ix);
                Print("  DER: <" & Length(der) & ">");
                local version := mbedTLS:X509CrtGetVersion(x509CertBin, ix);
                Print("  Version: " & version);
                local serialNumber := mbedTLS:X509CrtGetSerialNumber(x509CertBin, ix);
                Print("  Serial number: " & serialNumber);
                local subjectName := mbedTLS:X509CrtGetSubjectName(x509CertBin, ix);
                Print("  Subject name: " & subjectName);
                local issueDate := mbedTLS:X509CrtGetIssueDate(x509CertBin, ix);
                Print("  Issue date: " & DateNTime(issueDate));
                local expirationDate := mbedTLS:X509CrtGetExpirationDate(x509CertBin, ix);
                Print("  Expiration date: " & DateNTime(expirationDate));
                local signatureAlgorithm := mbedTLS:X509CrtGetSignatureAlgorithm(x509CertBin, ix);
                Print("  Signature algorithm: " & signatureAlgorithm);
            end;
            packageCursor:move(1);
        end;
        :TestEndCb(true);
    end
});

DefConst('kNetworkTest, {
    fEndpoint: NIL,
    fRemoteAddress: NIL,
    fRemotePort: NIL,
    fRemoteHost: NIL,
    fSuccess: NIL,
    Run: func()
    begin
        :SetStatus("Resolving domain");
        try
            DNSGetAddressFromName(fRemoteHost, self, 'GetAddressCb)
        onexception |evt| do begin
            :SetStatus("Exception with DNSGetAddressFromName: " & CurrentException().name);
            :TestEndCb(nil)
        end
    end,
    GetAddressCb: func(dnsResultArray, dnsResultCode)
    begin
        if dnsResultArray and Length(dnsResultArray) > 0 then
        begin
            fRemoteAddress := dnsResultArray[0].resultIPAddress;
            call kPrintObject with (fRemoteAddress);
            AddDeferredSend(self, 'Instantiate, [])
        end else begin
            :SetStatus("DNS resolution failed");
            :TestEndCb(nil);
        end
    end,
    Instantiate: func()
    begin
        :SetStatus("Instantiating endpoint");
        fEndpoint := {_proto: protoBasicEndpoint, _parent: self};
        try
            fEndpoint:instantiate(fEndpoint, :BuildInstantiateOptions());
            AddDeferredSend(self, 'Bind, [])
        onexception |evt| do begin
            :SetStatus("Exception with Instantiate: " & CurrentException().name);
            :Dispose()
        end
    end,
    BuildInstantiateOptions: func()
    begin
        return [
            {label: "inet", type: 'service, opCode: opSetRequired, result: nil},
            {label: "ilid", type: 'option, opCode: opSetRequired, result: nil,
            form: 'template, data: {arglist: [fLinkID], typeList: [kStruct, kUlong]}},
            {label: "itsv", type: 'option, opCode: opSetRequired, result: nil,
            form: 'template, data: {arglist: [1], typeList: [kStruct, kUlong]}}
        ]
    end,
    Bind: func()
    begin
        try
            fEndpoint:Bind([], nil);
            AddDeferredSend(self, 'Connect, [])
        onexception |evt| do begin
            :SetStatus("Exception with Bind: " & CurrentException().name);
            :Dispose()
        end
    end,
    Connect: func()
    begin
        :SetStatus("Connecting");
        try
            fEndpoint:Connect(:BuildConnectOptions(), nil);
            AddDeferredSend(self, 'SendRequest, [])
        onexception |evt| do begin
            :SetStatus("Exception with Connect: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Unbind()
        end
    end,
    BuildConnectOptions: func()
    begin
        return [
            {label: "itrs", type: 'option, opCode: opSetRequired, result: nil,
            form: 'template, data: {arglist: [fRemoteAddress, fRemotePort],
            typeList: [kStruct, [kArray, kByte, 4], kShort]}},
            {label: "iprf", type: 'option, opCode: opGetCurrent, result: nil,
            form: 'template, data: {arglist: [[0, 0, 0, 0], [0, 0, 0, 0]],
            typeList: [kStruct, [kArray, kByte, 4], [kArray, kByte, 4]]}},
        ]
    end,
    Disconnect: func()
    begin
        if fEndpoint = nil then return;
        :SetStatus("Disconnecting");
        try
            fEndpoint:Disconnect(true, nil);
            AddDeferredSend(self, 'Unbind, [])
        onexception |evt| do begin
            fSuccess := nil;
            :SetStatus("Exception with Disconnect: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Unbind()
        end
    end,
    Unbind: func()
    begin
        if fEndpoint = nil then return;
        :SetStatus("Unbinding");
        try
            fEndpoint:Unbind(nil);
            AddDeferredSend(self, 'Dispose, []);
        onexception |evt| do begin
            fSuccess := nil;
            :SetStatus("Exception with Unbind: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Dispose();
        end
    end,
    Dispose: func()
    begin
        if fEndpoint = nil then return;
        :SetStatus("Disposing");
        try
            fEndpoint:Dispose();
            fEndpoint := nil;
            AddDeferredSend(self, 'TestEndCb, [fSuccess])
        onexception |evt| do begin
            :SetStatus("Exception with Dispose: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :TestEndCb(nil)
        end
    end
});

DefConst('kHTTPRequestTest, {
    _proto: kNetworkTest,
    fRemotePort: 80,
    fRemoteHost: "www.apple.com",
    fSuccessTestLine: "Location: https://www.apple.com/" & unicodeCR & unicodeLF,
    fLineCount: 0,
    SendRequest: func()
    begin
        :SetStatus("Sending request");
        try
            fEndpoint:Output(
                "GET / HTTP/1.0" & unicodeCR & unicodeLF &
                "Host: " & fRemoteHost & unicodeCR & unicodeLF &
                unicodeCR & unicodeLF, nil, nil);
            AddDeferredSend(self, 'RecvResponse, [])
        onexception |evt| do begin
            :SetStatus("Exception with Output: " & CurrentException().name);
            :Disconnect()
        end
    end,
    RecvResponse: func()
    begin
        local inputSpec;
        :SetStatus("Receiving response");
        try
            inputSpec := {
                _parent: self,
                form: 'string,
                termination: {endSequence: unicodeCR & unicodeLF},
                InputScript: func(ep, data, terminator, options)
                begin
                    local printLine;
                    if :TestLine(data) then fSuccess := true;
                    printLine := data;
                    if terminator <> nil and terminator.condition = 'endSequence then
                        printLine := SubStr(printLine, 0, StrLen(printLine) - 2);
                    fLineCount := fLineCount + 1;
                    :SetStatus("Received line " & fLineCount & ":\n" & printLine & "\n")
                end,
                CompletionScript: func(ep, options, result)
                begin
                    if result = -16009 or result = -16013 then
                        :SetStatus("Remote disconnected")
                    else begin
                        :SetStatus("Completion result: " & result);
                        fSuccess := nil;
                    end;
                    AddDelayedSend(self, 'Disconnect, [], 1000);
                end
            };
            fEndpoint:SetInputSpec(inputSpec)
        onexception |evt| do begin
            :SetStatus("Exception with SetInputSpec: " & CurrentException().name);
            fSuccess := nil;
            :Disconnect()
        end
    end,
    TestLine: func(line)
    begin
        return StrEqual(line, fSuccessTestLine)
    end,
});

DefConst('kHTTPSRequestAppleTest,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 443,
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    BuildConnectOptions: func()
    begin
        local options := Clone(inherited:BuildConnectOptions());
        AddArraySlot(options, {
                label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [true], typelist: [kStruct, kBoolean,]}
        });
        AddArraySlot(options, {
                label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [fRemoteHost], typelist: [ kStruct, [kArray, kChar, 0]] }
        });
        return options;
    end
});

DefConst('kHTTPRequestLocalhostTest,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 8080,
    fRemoteHost: "127.0.0.1",
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF
});

DefConst('kHTTPSRequestLocalhostTestPEM,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 4433,
    fRemoteHost: "127.0.0.1",
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    BuildConnectOptions: func()
    begin
        local options := Clone(inherited:BuildConnectOptions());
        AddArraySlot(options, {
                label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: {
                    arglist: [true],
                    typelist: [kStruct, kBoolean,]
                }
        });
        AddArraySlot(options, {
                label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: ["localhost"], typelist: [ kStruct, [kArray, kChar, 0]] }
        });
        // For PEM, length must include the ending null terminator (which the communication layer will add)
        AddArraySlot(options, {
                label: kCMOSSLCertificateAuthority, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [StrLen(kTestCAPEMStr) + 1, 0, kTestCAPEMStr], typelist: [kStruct, kULong, kULong, [kArray, kChar, 0]] }
        });
        return options;
    end
});

DefConst('kHTTPSRequestLocalhostTestDER,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 4433,
    fRemoteHost: "127.0.0.1",
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    BuildConnectOptions: func()
    begin
        local options := Clone(inherited:BuildConnectOptions());
        AddArraySlot(options, {
                label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: {
                    arglist: [true],
                    typelist: [kStruct, kBoolean,]
                }
        });
        AddArraySlot(options, {
                label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: ["localhost"], typelist: [ kStruct, [kArray, kChar, 0]] }
        });
        // For DER, add certificates one by one
        AddArraySlot(options, {
                label: kCMOSSLCertificateAuthority, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [Length(kTestCADER_SHA256), 0, kTestCADER_SHA256], typelist: [kStruct, kULong, kULong, [kArray, kByte, Length(kTestCADER_SHA256)]] }
        });
        AddArraySlot(options, {
                label: kCMOSSLCertificateAuthority, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [Length(kTestCADER_SHA1), 0, kTestCADER_SHA1], typelist: [kStruct, kULong, kULong, [kArray, kByte, Length(kTestCADER_SHA1)]] }
        });
        AddArraySlot(options, {
                label: kCMOSSLCertificateAuthority, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [Length(kTestCADER_EC), 0, kTestCADER_EC], typelist: [kStruct, kULong, kULong, [kArray, kByte, Length(kTestCADER_EC)]] }
        });
        return options;
    end
});

DefConst('kHTTPSRequestLocalhostTestNoAuth,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 4433,
    fRemoteHost: "127.0.0.1",
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    BuildConnectOptions: func()
    begin
        local options := Clone(inherited:BuildConnectOptions());
        AddArraySlot(options, {
                label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: {
                    arglist: [true],
                    typelist: [kStruct, kBoolean,]
                }
        });
        AddArraySlot(options, {
                label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: ["localhost"], typelist: [ kStruct, [kArray, kChar, 0]] }
        });
        AddArraySlot(options, {
                label: kCMOSSLVerifyMode, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [MBEDTLS_SSL_VERIFY_NONE], typelist: [kStruct, kULong] }
        });
        return options;
    end
});

DefConst('kHTTPSRequestLocalhostTestBogus,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 4433,
    fRemoteHost: "127.0.0.1",
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    Connect: func()
    begin
        :SetStatus("Connecting (async)");
        try
            local result := fEndpoint:Connect(:BuildConnectOptions(), {
                _parent: self,
                async: true,
                CompletionScript: func(endpoint, options, result)
                begin
                    Print("Connect's CompletionScript called");
                    foreach option in options do call kPrintObject with (option);
                    call kPrintObject with (result);
                    if result = nil then
                        AddDeferredSend(self, 'SendRequest, [])
                    else begin
                        :SetStatus("Connect failed");
                        :Unbind()
                    end
                end
            });
            foreach r in result do
                call kPrintObject with (r);
        onexception |evt| do begin
            :SetStatus("Exception with Connect: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Unbind()
        end
    end,
    BuildConnectOptions: func()
    begin
        local options := Clone(inherited:BuildConnectOptions());
        AddArraySlot(options, {
                label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: {
                    arglist: [true],
                    typelist: [kStruct, kBoolean,]
                }
        });
        AddArraySlot(options, {
                label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: ["localhost"], typelist: [ kStruct, [kArray, kChar, 0]] }
        });
        local BogusCert := [0, 0, 0, 0, 0];
        AddArraySlot(options, {
                label: kCMOSSLCertificateAuthority, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                data: { arglist: [Length(BogusCert), 0, BogusCert], typelist: [kStruct, kULong, kULong, [kArray, kByte, Length(BogusCert)]] }
        });
        return options;
    end
});

DefConst('kHTTPSRequestAppleGetPeerCert,
{
    _proto: kHTTPSRequestAppleTest,
    mbedTLS: UR(kMbedTLSSymbol, 'mbedTLS),
    Connect: func()
    begin
        :SetStatus("Connecting");
        try
            fEndpoint:Connect(:BuildConnectOptions(), nil);
            AddDeferredSend(self, 'GetPeerCertificate, [])
        onexception |evt| do begin
            :SetStatus("Exception with Connect: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Unbind()
        end
    end,
    GetPeerCertificate: func()
    begin
        try
            local certIx := 0;
            loop begin
                local option := {
                    label: kCMOSSLGetPeerCertificate, type: 'option, opCode: opGetCurrent, result: nil, form: 'template,
                    data: {arglist: [0, certIx], typelist: [kStruct, kULong, kULong]}
                };
                local result := fEndpoint:Option([option], nil);
                if result[0].result <> NIL then break;
                local size := result[0].data.arglist[0];
                if size = 0 then break;
                :SetStatus("Getting information about certifiate #" & certIx);
                local certDER := Array(size, 0);
                option.data := {arglist: [size, certIx, certDER], typelist: [kStruct, kULong, kULong, [kArray, kByte, size]]};
                result := fEndpoint:Option([option], nil);
                certDER := result[0].data.arglist[2];
                local x509CertBin := mbedTLS:X509CrtNew();
                mbedTLS:X509CrtParse(x509CertBin, certDER);
                local version := mbedTLS:X509CrtGetVersion(x509CertBin, 0);
                Print("Version: " & version);
                local serialNumber := mbedTLS:X509CrtGetSerialNumber(x509CertBin, 0);
                Print("Serial number: " & serialNumber);
                local subjectName := mbedTLS:X509CrtGetSubjectName(x509CertBin, 0);
                Print("Subject name: " & subjectName);
                local issueDate := mbedTLS:X509CrtGetIssueDate(x509CertBin, 0);
                Print("Issue date: " & DateNTime(issueDate));
                local expirationDate := mbedTLS:X509CrtGetExpirationDate(x509CertBin, 0);
                Print("Expiration date: " & DateNTime(expirationDate));
                local signatureAlgorithm := mbedTLS:X509CrtGetSignatureAlgorithm(x509CertBin, 0);
                Print("Signature algorithm: " & signatureAlgorithm);
                certIx := certIx + 1;
            end;
            fSuccess := certIx > 0;
            AddDeferredSend(self, 'Disconnect, [])
        onexception |evt| do begin
            :SetStatus("Exception with GetPeerCertificate: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Disconnect()
        end
    end
});

DefConst('kHTTPSRequestAppleUpgradeTest,
{
    _proto: kHTTPRequestTest,
    fRemotePort: 443,
    fSuccessTestLine: "HTTP/1.0 200 OK" & unicodeCR & unicodeLF,
    Connect: func()
    begin
        :SetStatus("Connecting");
        try
            fEndpoint:Connect(:BuildConnectOptions(), nil);
            AddDeferredSend(self, 'Upgrade, [])
        onexception |evt| do begin
            :SetStatus("Exception with Connect: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Unbind()
        end
    end,
    Upgrade: func()
    begin
        :SetStatus("Upgrading");
        try
            local hostnameOption := {
                    label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                    data: { arglist: [fRemoteHost], typelist: [ kStruct, [kArray, kChar, 0]] }
                };
            local sslEnableOption :=
                {
                    label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                    data: { arglist: [true], typelist: [kStruct, kBoolean,]}
                };
            local result := fEndpoint:Option([hostnameOption], nil);
            call kPrintObject with (result);
            result := fEndpoint:Option([sslEnableOption], nil);
            call kPrintObject with (result);
            AddDeferredSend(self, 'SendRequest, [])
        onexception |evt| do begin
            :SetStatus("Exception with Upgrade: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Disconnect()
        end
    end
});

DefConst('kHTTPRequestAppleBogusUpgradeTest,
{
    _proto: kHTTPSRequestAppleUpgradeTest,
    fRemotePort: 80
});

DefConst('kIMAPSWithStartTLS,
{
    _proto: kNetworkTest,
    fRemotePort: 143,
    fRemoteHost: "imap.outlook.com",
    fSuccessTestLine: "Location: https://www.apple.com/" & unicodeCR & unicodeLF,
    fLineCount: 0,
    SendRequest: func()
    begin
        :SetStatus("Sending request");
        try
            fEndpoint:Output(
                "0 STARTTLS" & unicodeCR & unicodeLF, nil, nil);
            AddDeferredSend(self, 'RecvStartTLSResponse, [])
        onexception |evt| do begin
            :SetStatus("Exception with Output: " & CurrentException().name);
            :Disconnect()
        end
    end,
    RecvStartTLSResponse: func()
    begin
        local inputSpec;
        :SetStatus("Receiving STARTTLS response");
        try
            inputSpec := {
                _parent: self,
                form: 'string,
                termination: {endSequence: unicodeCR & unicodeLF},
                InputScript: func(ep, data, terminator, options)
                begin
                    if BeginsWith(data, "0 OK") then
                    begin
                        fEndpoint:SetInputSpec(nil);
                        AddDeferredSend(self, 'Upgrade, []);
                    end;
                    local printLine := data;
                    if terminator <> nil and terminator.condition = 'endSequence then
                        printLine := SubStr(printLine, 0, StrLen(printLine) - 2);
                    :SetStatus("Received line:\n" & printLine & "\n")
                end,
                CompletionScript: func(ep, options, result)
                begin
                    if result = -16009 or result = -16013 then
                        :SetStatus("Remote disconnected")
                    else begin
                        :SetStatus("Completion result: " & result);
                        fSuccess := nil;
                    end;
                    AddDelayedSend(self, 'Disconnect, [], 1000);
                end
            };
            fEndpoint:SetInputSpec(inputSpec)
        onexception |evt| do begin
            :SetStatus("Exception with SetInputSpec: " & CurrentException().name);
            fSuccess := nil;
            :Disconnect()
        end
    end,
    Upgrade: func()
    begin
        :SetStatus("Upgrading");
        try
            local hostnameOption := {
                    label: kCMOSSLHostname, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                    data: { arglist: [fRemoteHost], typelist: [ kStruct, [kArray, kChar, 0]] }
                };
            local sslEnableOption :=
                {
                    label: kCMOSSLEnable, type: 'option, opCode: opSetRequired, result: nil, form: 'template,
                    data: { arglist: [true], typelist: [kStruct, kBoolean,]}
                };
            local result := fEndpoint:Option([hostnameOption], nil);
            call kPrintObject with (result);
            result := fEndpoint:Option([sslEnableOption], nil);
            call kPrintObject with (result);
            AddDeferredSend(self, 'SendLogout, [])
        onexception |evt| do begin
            :SetStatus("Exception with Upgrade: " & CurrentException().name);
            call kPrintObject with (CurrentException());
            :Disconnect()
        end
    end,
    SendLogout: func()
    begin
        :SetStatus("Sending LOGOUT");
        try
            fEndpoint:Output(
                "1 LOGOUT" & unicodeCR & unicodeLF, nil, nil);
            AddDeferredSend(self, 'RecvLogoutResponse, [])
        onexception |evt| do begin
            :SetStatus("Exception with Output: " & CurrentException().name);
            :Disconnect()
        end
    end,
    RecvLogoutResponse: func()
    begin
        local inputSpec;
        :SetStatus("Receiving LOGOUT response");
        try
            inputSpec := {
                _parent: self,
                form: 'string,
                termination: {endSequence: unicodeCR & unicodeLF},
                InputScript: func(ep, data, terminator, options)
                begin
                    if BeginsWith(data, "1 OK") then
                    begin
                        fSuccess := true;
                        AddDeferredSend(self, 'Disconnect, []);
                    end;
                    local printLine := data;
                    if terminator <> nil and terminator.condition = 'endSequence then
                        printLine := SubStr(printLine, 0, StrLen(printLine) - 2);
                    :SetStatus("Received line:\n" & printLine & "\n")
                end,
                CompletionScript: func(ep, options, result)
                begin
                    if result = -16009 or result = -16013 then
                        :SetStatus("Remote disconnected")
                    else if result = -16005 then
                        :SetStatus("We're disconnecting")
                    else begin
                        :SetStatus("Completion result: " & result);
                        fSuccess := nil;
                    end;
                    AddDelayedSend(self, 'Disconnect, [], 1000);
                end
            };
            fEndpoint:SetInputSpec(inputSpec)
        onexception |evt| do begin
            :SetStatus("Exception with SetInputSpec: " & CurrentException().name);
            fSuccess := nil;
            :Disconnect()
        end
    end
});

DefConst('kTestLabelCommands,
[
    {item: "Parse PEM certificates", test: kPEMParseTest},
    {item: "Parse DER certificates", test: kDERParseTest},
    {item: "Parse Root certificates", test: kParseRootCertificatesTest},
    'pickSeparator,
    {item: "HTTP www.apple.com", test: kHTTPRequestTest},
    {item: "HTTP localhost:8080", test: kHTTPRequestLocalhostTest},
    'pickSeparator,
    {item: "HTTPS www.apple.com", test: kHTTPSRequestAppleTest},
    {item: "HTTPS localhost:4433 with PEM", test: kHTTPSRequestLocalhostTestPEM},
    {item: "HTTPS localhost:4433 with DER", test: kHTTPSRequestLocalhostTestDER},
    {item: "HTTPS localhost:4433 with no auth", test: kHTTPSRequestLocalhostTestNoAuth},
    'pickSeparator,
    {item: "HTTPS www.apple.com with get peer cert", test: kHTTPSRequestAppleGetPeerCert},
    {item: "HTTPS www.apple.com with upgrade", test: kHTTPSRequestAppleUpgradeTest},
    {item: "IMAP imap.outlook.com with STARTTLS", test: kIMAPSWithStartTLS},
    'pickSeparator,
    {item: "HTTPS localhost:4433 with bogus cert", test: kHTTPSRequestLocalhostTestBogus},
    {item: "HTTP www.apple.com with bogus upgrade", test: kHTTPRequestAppleBogusUpgradeTest},
]);
