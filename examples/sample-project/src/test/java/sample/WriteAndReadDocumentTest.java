package sample;

import org.junit.Test;

import com.marklogic.client.io.StringHandle;
import com.marklogic.junit.Fragment;

public class WriteAndReadDocumentTest extends AbstractSampleProjectTest {

    /**
     * This is a basic test - it loads an XML document using the MarkLogic Java API, and then it reads that document
     * back as a String using the MarkLogic Java API. That String is then parsed as an XML fragment, and with the
     * Fragment object, we can easily make a number of assertions.
     */
    @Test
    public void writeAndReadDocument() {
        loadPerson("/jane.xml", "Jane", "This is a sample document");

        String xml = getClient().newXMLDocumentManager().read("/jane.xml", new StringHandle()).get();
        Fragment frag = parse(xml);
        frag.assertElementValue("This is a basic assertion on the text node of a particular element",
                "/sample:person/sample:name", "Jane");
        frag.assertElementExists("Can also assert on the presence of an element as expressed via XPath",
                "//sample:description[. = 'This is a sample document']");
        assertFragmentIsIdentical(
                "This is an example of using XMLUnit libraries to assert on the equality of XML fragments", frag,
                readTestClassResource("jane-sample.xml"));

        // Example of using prettyPrint for debugging
        frag.prettyPrint();
    }

}
