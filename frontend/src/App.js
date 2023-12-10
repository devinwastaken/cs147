import logo from './logo.svg';
import './App.css';
import { useEffect, useState } from 'react';
import LineChart from './LineChart';

function App() {
  const [tempData, setTempData] = useState([])
  const [lightData, setLightData] = useState([])

  useEffect(() => {
    const fetchData = async () => {
      const tempResponse = await fetch('http://localhost:3000/data/temp.csv')
      const lightResponse = await fetch('http://localhost:3000/data/light.csv')

      const tempResponseText = await tempResponse.text()
      const lightResponseText = await lightResponse.text()

      const tempCsv = parseCsv(tempResponseText)
      const lightCsv = parseCsv(lightResponseText)

      setTempData(tempCsv)
      setLightData(lightCsv)
    }

    fetchData()
  }, [])

  const parseCsv = (csv) => {
    const rows = csv.split('\n')
    const parsedCsv = rows.map(row => row.split(','))
    return parsedCsv
  }

  console.log(tempData)
  console.log(lightData)

  return (
    <div className="App">
      <header className="App-header">
        <LineChart data={tempData} name={'Temperature'} />
        <LineChart data={lightData} name={'Light'} />
      </header>
    </div>
  );
}

export default App;
