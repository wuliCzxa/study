LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;
USE IEEE.STD_LOGIC_UNSIGNED.ALL;

ENTITY ALU IS
PORT (
  A         : IN    STD_LOGIC_VECTOR(15 DOWNTO 0);  -- K3, K2
  W         : IN    STD_LOGIC_VECTOR(15 DOWNTO 0);  -- K1, K0
  S0        : IN    STD_LOGIC;                      -- K4.0
  S1        : IN    STD_LOGIC;                      -- K4.1
  S2        : IN    STD_LOGIC;                      -- K4.2
  D         : OUT   STD_LOGIC_VECTOR(15 DOWNTO 0);  -- S1, S0
  CIn       : IN    STD_LOGIC;                      -- K4.3
  COut      : OUT   STD_LOGIC                       -- A0
);
END ALU;

ARCHITECTURE behv OF ALU IS

SIGNAL T: STD_LOGIC_VECTOR(16 DOWNTO 0);  

BEGIN

  T <= ('0' & A)   + ('0' & W)       WHEN S2 = '0' AND S1 = '0' AND S0 = '0' ELSE
       ('0' & A)   - ('0' & W)       WHEN S2 = '0' AND S1 = '0' AND S0 = '1' ELSE
       ('0' & A)  OR ('0' & W)       WHEN S2 = '0' AND S1 = '1' AND S0 = '0' ELSE
       ('0' & A) AND ('0' & W)       WHEN S2 = '0' AND S1 = '1' AND S0 = '1' ELSE
       ('0' & A)   + ('0' & W) + CIn WHEN S2 = '1' AND S1 = '0' AND S0 = '0' ELSE
       ('0' & A)   - ('0' & W) - CIn WHEN S2 = '1' AND S1 = '0' AND S0 = '1' ELSE
       NOT ('0' & A)                 WHEN S2 = '1' AND S1 = '1' AND S0 = '0' ELSE
       ('0' & A);

  D    <= T(15 DOWNTO 0);
  COut <= T(16);
  
END behv;

